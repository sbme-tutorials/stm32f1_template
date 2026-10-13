#ifndef RCC_PRIVATE_H
#define RCC_PRIVATE_H

#include "Std_Types.h"

#define RCC_BASE_ADDR 0x40021000UL
#define RCC_CR       (*(volatile uint32 *)(RCC_BASE_ADDR + 0x00UL))
#define RCC_CFGR     (*(volatile uint32 *)(RCC_BASE_ADDR + 0x04UL))
#define RCC_AHBENR   (*(volatile uint32 *)(RCC_BASE_ADDR + 0x14UL))
#define RCC_APB2ENR  (*(volatile uint32 *)(RCC_BASE_ADDR + 0x18UL))
#define RCC_APB1ENR  (*(volatile uint32 *)(RCC_BASE_ADDR + 0x1CUL))

#endif
