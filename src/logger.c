#include <stdint.h>
#include "../inc/logger.h"

void SPI_GPIO_init(){
    GPIOA_MODER &= ~( (3U << (5 * 2)) |
        (3U << (6 * 2)) |
        (3U << (7 * 2)) );

    GPIOA_MODER |=  ( (2U << (5 * 2)) |
        (2U << (6 * 2)) |
        (2U << (7 * 2)) );

    // AFRL → set AF5
    GPIOA_AFRL &= ~( (0xFU << (5 * 4)) |
        (0xFU << (6 * 4)) |
        (0xFU << (7 * 4)) );

    GPIOA_AFRL |=  ( (5U << (5 * 4)) |
        (5U << (6 * 4)) |
        (5U << (7 * 4)) );

    // OSPEEDR → high speed (11)
    GPIOA_OSPEEDR |= ( (3U << (5 * 2)) |
        (3U << (6 * 2)) |
        (3U << (7 * 2)) );

    // OTYPER → push-pull
    GPIOA_OTYPER &= ~( (1U << 5) |
        (1U << 6) |
        (1U << 7) );

    // PUPDR → no pull
    GPIOA_PUPDR &= ~( (3U << (5 * 2)) |
        (3U << (6 * 2)) |
        (3U << (7 * 2)) );
}

void SPI_init(){
    RCC_AHB1ENR |= (1U << 0);
    RCC_APB2ENR |= (1U << 12);

    SPI_GPIO_init();

    SPI1_CR1 = 0;

    SPI1_CR1 |= SPI_CR1_MSTR;      // master mode
    SPI1_CR1 |= SPI_CR1_BR_DIV16;  // baud rate prescaler
    SPI1_CR1 |= SPI_CR1_SSM;       // software slave management
    SPI1_CR1 |= SPI_CR1_SSI;       // internal NSS high
    // CPOL = 0, CPHA = 0 by default (Mode 0)
    // LSBFIRST = 0 (default)

    SPI1_CR1 |= SPI_CR1_SPE;
    SPI1_CR2 = 0;
}

void SPI_write_byte(uint8_t data) {
    uint32_t timer = 100000; // timer to avoid getting stuck at SPI Write
    while (!(SPI1_SR & SPI_SR_TXE) && --timer > 0);
    if(timer == 0) return;
    SPI1_DR = data;

    timer = 100000;
    while (SPI1_SR & SPI_SR_BSY && --timer > 0);
    if(timer == 0) return;
    volatile uint8_t dummy = SPI1_DR;
}

void SPI_write(char *str) {
    while (*str) {
        SPI_write_byte(*str++);
    }
}
