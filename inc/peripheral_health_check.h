#ifndef _PERIPHERAL_HEALTH_CHECK_H_
#define _PERIPHERAL_HEALTH_CHECK_H_
#include <stdint.h>

#define RCC_APB1ENR      (*(volatile uint32_t*)0x4002101C)
#define RCC_APB1ENR_TIM7EN (1 << 5)

#define TIM7_BASE  0x40001400
#define TIM7_CR1   (*(volatile uint32_t*)(TIM7_BASE + 0x00))
#define TIM7_DIER  (*(volatile uint32_t*)(TIM7_BASE + 0x0C))
#define TIM7_SR    (*(volatile uint32_t*)(TIM7_BASE + 0x10))
#define TIM7_PSC   (*(volatile uint32_t*)(TIM7_BASE + 0x28))
#define TIM7_ARR   (*(volatile uint32_t*)(TIM7_BASE + 0x2C))

#define NVIC_ISER0 (*(volatile uint32_t*)0xE000E100)

extern volatile int system_tick;
extern volatile int last_battery_tick;
extern volatile int last_egg_tick;

void peripheral_watcher_init(void);
void peripheral_watcher_IRQ(void);

#endif
