#include <stdint.h>
#include <stdio.h>
#include "scheduler.h"

#define USART1 0x40011000
#define USART2 0x40004400
#define USART3 0x40004800

#define MAX_BATTERY_TEMP 80
#define MAX_BATTER_CURRENT 20
#define MIN_BATTER_CURRENT 15
#define MAX_ENGINE_TEMP 80
#define MAX_ENGINE_CURRENT 17
#define MIN_ENGINE_CURRENT 12

#define STATUS_REG(x) (*(volatile uint32_t *)((x) + 0x00))
#define DATA_REG(x)   (*(volatile uint32_t *)((x) + 0x04))
#define BAUD_RATE_REG(x) (*(volatile uint32_t *)((x) + 0x08))
#define CONTROL_REG(x)   (*(volatile uint32_t *)((x) + 0x0C))

// control registor bits functions
//[13]  UE          USART Enable
//[3]   TE          Transmitter Enable
//[2]   RE          Receiver Enable

// status register bits function
// [5] RXNE check for data read
// [7] check for data write

#define RXNE (1<<5)
#define TXE  (1<<7)
#define UE   (1<<13)
#define TE   (1<<3)
#define RE   (1<<2)

#define BUFF_SIZE 64

static int malfunction = 0;
char reader_buff_2[BUFF_SIZE];
char reader_buff_3[BUFF_SIZE];
char writer_buff_2[BUFF_SIZE];
char writer_buff_3[BUFF_SIZE];

static void uart_init(uint32_t base) {
    BAUD_RATE_REG(base) = 0x683;
    CONTROL_REG(base) = UE | TE | RE;
}

static void uart_log(uint32_t base, const char *str) {
    while (*str) {
        while (!(STATUS_REG(base) & TXE));
        DATA_REG(base) = *str;
        str++;
    }
}

static int uart_read_available(uint32_t base){
    return STATUS_REG(base) & RXNE;
}

static char uart_c_read(uint32_t base) {
    while (!(STATUS_REG(base) & RXNE));
    return DATA_REG(base);
}

static void uart_buff_reader(uint32_t base, char *buffer, int *index) {
    if(uart_read_available(base)) {
        char c = uart_c_read(base);

        if(c == '\n') {
            buffer[*index] = '\0';
            *index = 0;
        }
        else if(*index < BUFF_SIZE - 1) {
            buffer[(*index)++] = c;
        }
        else {
            buffer[*index] = '\0';
            *index = 0;
        }
    }
}

static void data_parser(char* buff, int* a, int* b){
    int read_data_count = 0;
    int curr_data = 0;

    while(*buff){
        if(*buff >= '0' && *buff <= '9'){
            curr_data = curr_data * 10 + (*buff - '0');
        }
        else{
            if(read_data_count == 0){
                *a = curr_data;
                read_data_count = 1;
            }
            else{
                *b = curr_data;
                return;
            }
            curr_data = 0;
        }
        buff++;
    }
}

static void malfunction_detector(const char *device, char* buff){
    uart_log(USART1, device);
    uart_log(USART1, buff);
}

static void task_battery_malfunction(){
    malfunction_detector("Battery Fault: ", reader_buff_2);
    update_priority(task_battery_malfunction, 0);
    scheduler_unready(task_battery_malfunction);
    reader_buff_2[0] = '\0';
}

static void task_engine_malfunction(){
    malfunction_detector("Engine Fault: ", reader_buff_3);
    update_priority(task_engine_malfunction, 0);
    scheduler_unready(task_engine_malfunction);
    reader_buff_3[0] = '\0';
}

static int idx2 = 0;
static void task_uart2_reader(){
    uart_buff_reader(USART2, reader_buff_2, &idx2);
    if(reader_buff_2[0] != '\0'){
        int t = -1, c = -1;
        data_parser(reader_buff_2, &t, &c);

        if(t == -1 || c == -1){
            uart_log(USART1, "Cant read battery data\n");
        }
        else if(t > MAX_BATTERY_TEMP || c > MAX_BATTER_CURRENT){
            update_priority(task_battery_malfunction, 2);
            scheduler_ready(task_battery_malfunction);
        }
    }
}

static int idx3 = 0;
static void task_uart3_reader(){
    uart_buff_reader(USART3, reader_buff_3, &idx3);
    if(reader_buff_3[0] != '\0'){
        int t = -1, c = -1;
        data_parser(reader_buff_3, &t, &c);

        if(t == -1 || c == -1){
            uart_log(USART1, "Cant read engine data\n");
        }
        else if(t > MAX_ENGINE_TEMP || c > MAX_ENGINE_CURRENT){
            update_priority(task_engine_malfunction, 2);
            scheduler_ready(task_engine_malfunction);
        }
    }
}

int main(void) {
    uart_init(USART1);
    uart_init(USART2);
    uart_init(USART3);
    uart_log(USART1, "MCU Running\n");

    scheduler_init();

    insert_task(task_uart2_reader, 1);
    insert_task(task_uart3_reader, 1);
    insert_task(task_battery_malfunction, 0);
    insert_task(task_engine_malfunction, 0);
    scheduler_unready(task_battery_malfunction);
    scheduler_unready(task_engine_malfunction);

    while(1){
        tcb_t *t = get_task();
        if(t) t->fn();
    }

    return 0;
}
