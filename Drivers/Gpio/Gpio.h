#ifndef GPIO_H
#define GPIO_H

#include "Std_Types.h"

/* PortName */
#define GPIO_A 'A'
#define GPIO_B 'B'
#define GPIO_C 'C'
#define GPIO_D 'D'

/* PinMode: API values match stm32f4-sec10, not the F1 register encoding. */
#define GPIO_INPUT  0x00U
#define GPIO_OUTPUT 0x01U
#define GPIO_AF     0x02U
#define GPIO_ANALOG 0x03U

/* DefaultState for output / alternate-function output */
#define GPIO_PUSH_PULL  0x00U
#define GPIO_OPEN_DRAIN 0x01U

/* DefaultState for input */
#define GPIO_NO_PULL_DOWN 0x00U
#define GPIO_PULL_UP      0x01U
#define GPIO_PULL_DOWN    0x02U

#define LOW  0x00U
#define HIGH 0x01U
#define OK   0x00U
#define NOK  0x01U

/* Enable the port clock with Rcc_Enable before accessing GPIO registers.
 * Outputs use 2 MHz speed. F1 alternate-function remapping uses AFIO;
 * there is no F4-style Gpio_SetAF / AFR register on this device. */
void Gpio_Init(uint8 PortName, uint8 PinNumber, uint8 PinMode, uint8 DefaultState);

uint8 Gpio_WritePin(uint8 PortName, uint8 PinNumber, uint8 Data);

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNumber);

uint8 Gpio_TogglePin(uint8 PortName, uint8 PinNumber);

#endif
