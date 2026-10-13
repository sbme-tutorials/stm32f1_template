#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H

#include "Std_Types.h"

#define GPIOA_BASE_ADDR 0x40010800UL
#define GPIOB_BASE_ADDR 0x40010C00UL
#define GPIOC_BASE_ADDR 0x40011000UL
#define GPIOD_BASE_ADDR 0x40011400UL

typedef struct {
    volatile uint32 GPIO_CRL;  /* 0x00: configuration for pins 0..7 */
    volatile uint32 GPIO_CRH;  /* 0x04: configuration for pins 8..15 */
    volatile uint32 GPIO_IDR;  /* 0x08 */
    volatile uint32 GPIO_ODR;  /* 0x0C */
    volatile uint32 GPIO_BSRR; /* 0x10: set low half, reset high half */
    volatile uint32 GPIO_BRR;  /* 0x14 */
    volatile uint32 GPIO_LCKR; /* 0x18 */
} GpioType;

#endif
