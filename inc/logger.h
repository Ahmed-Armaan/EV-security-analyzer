#ifndef _LOGGER_H
#define _LOGGER_H

#define RCC_BASE        0x40023800U
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x44U))

#define GPIOA_BASE      0x40020000U
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER    (*(volatile uint32_t *)(GPIOA_BASE + 0x04))
#define GPIOA_OSPEEDR   (*(volatile uint32_t *)(GPIOA_BASE + 0x08))
#define GPIOA_PUPDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))
#define GPIOA_AFRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x20))
#define GPIOA_AFRH      (*(volatile uint32_t *)(GPIOA_BASE + 0x24))

#define SPI1_BASE      0x40013000U
#define SPI1_CR1       (*(volatile uint32_t *)(SPI1_BASE + 0x00))
#define SPI1_CR2       (*(volatile uint32_t *)(SPI1_BASE + 0x04))
#define SPI1_SR        (*(volatile uint32_t *)(SPI1_BASE + 0x08))
#define SPI1_DR        (*(volatile uint32_t *)(SPI1_BASE + 0x0C))

#define SPI_CR1_CPHA     (1U << 0)
#define SPI_CR1_CPOL     (1U << 1)
#define SPI_CR1_MSTR     (1U << 2)
#define SPI_CR1_BR_DIV16 (3U << 3)
#define SPI_CR1_SPE      (1U << 6)
#define SPI_CR1_LSBFIRST (1U << 7)
#define SPI_CR1_SSI      (1U << 8)
#define SPI_CR1_SSM      (1U << 9)

#define SPI_SR_RXNE   (1U << 0)
#define SPI_SR_TXE    (1U << 1)
#define SPI_SR_BSY    (1U << 7)

void SPI_init(void);
void SPI_write(char *str);
//void SPI_read(void);

#endif
