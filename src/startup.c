#include <stdint.h>
#include "../inc/peripheral_health_check.h"

#define STACK_TOP 0x20001000

extern int main(void);
void Reset_Handler(void);
void TIM7_IRQHandler(void);

typedef void(*ISR)(void);

__attribute__((section(".isr_vector")))
ISR vectors[] = {
    (ISR)STACK_TOP,
    Reset_Handler,
    0, 0,
    0,0,0,0,0,0,0,
    0,
    0,0,
    0,
    0,
    /* IRQ0 - IRQ17 */
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,

    TIM7_IRQHandler,   // IRQ18
};

void Reset_Handler(void) {
    main();
}

void TIM7_IRQHandler(void) {
    peripheral_watcher_IRQ();
}
