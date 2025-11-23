#include <stdint.h>
#define STACK_TOP 0x20001000

extern int main(void);
void Reset_Handler(void);

__attribute__((section(".isr_vector")))
uint32_t vectors[] = {
    STACK_TOP,
    (uint32_t)Reset_Handler,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
};

void Reset_Handler(void) {
    main();
}
