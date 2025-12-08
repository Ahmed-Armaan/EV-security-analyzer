#include <stdint.h>
#include "../inc/peripheral_health_check.h"
#include "../inc/scheduler.h"

#define USART1 0x40011000

enum peripheral_failure_status{
    GOOD,
    ERROR,
    CRITICAL,
};

static volatile int curr_failure_state = GOOD;

volatile int system_tick = 0;
volatile int last_battery_tick = 0;
volatile int last_egg_tick = 0;
extern void task_spi_logger();

void peripheral_watcher_init(void){
    RCC_APB1ENR |= RCC_APB1ENR_TIM7EN;

    TIM7_PSC = 8000 - 1;
    TIM7_ARR = 1000 - 1;
    TIM7_DIER |= 1;     
    TIM7_CR1  |= 1;     
    NVIC_ISER0 |= (1 << 18);
    TIM7_SR &= ~1;
}

void peripheral_watcher_IRQ(void){
    TIM7_SR &= ~1;
    system_tick++;

    int delta_batt = system_tick - last_battery_tick;
    int delta_egg  = system_tick - last_egg_tick;

    if (delta_batt > 6 || delta_egg > 6) {
        curr_failure_state = CRITICAL;
    } else if (delta_batt > 3 || delta_egg > 3) {
        curr_failure_state = ERROR;
    } else {
        curr_failure_state = GOOD;
    }

    if (curr_failure_state == CRITICAL) {
        extern void uart_log(uint32_t base, const char *str);
        uart_log(USART1, "CRITICAL: peripheral failure\n");

        update_priority(task_spi_logger, 2);
        scheduler_ready(task_spi_logger);
    }
}
