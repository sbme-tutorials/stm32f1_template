#ifndef UTILS_H
#define UTILS_H

#include "Std_Types.h"

/* Blocking SysTick delay. Call Rcc_Init first (8 MHz core clock).
 * This helper owns SysTick and does not enable its interrupt. */
void DelayMs(uint32 Milliseconds);

#endif
