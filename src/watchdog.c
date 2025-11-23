#include <stdint.h>
#include "../inc/watchdog.h"

void watchdog_init(){
    IWDG_KR = 0x0000CCCC;
    IWDG_KR = 0x00005555;
    IWDG_PR = 0x00000003;
    IWDG_RLR = 0x0000FFFF;

    while(IWDG_SR & (1 << 0));
    while(IWDG_SR & (1 << 1));

    IWDG_KR = 0x0000AAAA;
}

void pet_watchdog(){
    IWDG_KR = 0x0000AAAA;
}
