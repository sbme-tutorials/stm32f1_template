#ifndef RCC_H
#define RCC_H

#include "Std_Types.h"

#define RCC_AHB  0UL
#define RCC_APB1 1UL
#define RCC_APB2 2UL

#define RCC_AFIO   (RCC_APB2 * 32UL + 0UL)
#define RCC_GPIOA  (RCC_APB2 * 32UL + 2UL)
#define RCC_GPIOB  (RCC_APB2 * 32UL + 3UL)
#define RCC_GPIOC  (RCC_APB2 * 32UL + 4UL)
#define RCC_GPIOD  (RCC_APB2 * 32UL + 5UL)
#define RCC_USART1 (RCC_APB2 * 32UL + 14UL)

#define RCC_HSI_FREQUENCY 8000000UL

/* Select the internal 8 MHz HSI and undivided core/APB1/APB2 clocks. */
void Rcc_Init(void);
void Rcc_Enable(uint8 PeripheralId);
void Rcc_Disable(uint8 PeripheralId);

#endif
