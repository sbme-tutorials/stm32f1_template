#ifndef PANDA_BOARD_H
#define PANDA_BOARD_H

#include "Gpio.h"

/* Panda Rev1 manual p. 15 schematic and PCB labels; the p. 16 prose
 * contradicts these. Carrier LEDs are active high. Ports and pin numbers
 * follow the custom Gpio driver API. */
#define PANDA_RED1_PORT GPIO_A
#define PANDA_RED1_PIN  2U
#define PANDA_RED2_PORT GPIO_A
#define PANDA_RED2_PIN  1U
#define PANDA_GREEN_PORT GPIO_C
#define PANDA_GREEN_PIN  15U
#define PANDA_BLUE_PORT GPIO_C
#define PANDA_BLUE_PIN  14U
#define PANDA_YELLOW_PORT GPIO_C
#define PANDA_YELLOW_PIN  13U

#define PANDA_BUZZER_PORT GPIO_B
#define PANDA_BUZZER_PIN  4U
#define PANDA_RELAY1_PORT GPIO_B
#define PANDA_RELAY1_PIN  5U
#define PANDA_RELAY2_PORT GPIO_A
#define PANDA_RELAY2_PIN  3U

/* PA9/PA10 route USART1 to the CH340 through the TX/RX DIP switches. */
#define PANDA_UART_TX_PIN 9U
#define PANDA_UART_RX_PIN 10U

/* Enable the required clocks, release PB4 from NJTRST and hold the buzzer low.
 * SWD remains available on PA13/PA14. Call once during startup. */
void Panda_DisableBuzzer(void);

#endif
