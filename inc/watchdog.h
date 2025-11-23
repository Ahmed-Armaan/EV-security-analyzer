#ifndef _IWDG_H_
#define _IWDG_H_

#define IWDG_KR (*(volatile uint32_t*)0x40003000)
#define IWDG_PR (*(volatile uint32_t*)0x40003004)
#define IWDG_RLR (*(volatile uint32_t*)0x40003008)
#define IWDG_SR (*(volatile uint32_t*)0x4000300C)

void watchdog_init();
void pet_watchdog();

#endif
