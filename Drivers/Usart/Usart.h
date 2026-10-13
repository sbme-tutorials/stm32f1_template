#ifndef USART_H
#define USART_H

#include "Std_Types.h"

#define Tx_OK  0U
#define Tx_NOK 1U

/* Call Rcc_Init first. USART1 uses PA9 TX / PA10 RX, 9600 baud, 8N1.
 * Polling only: no interrupts, DMA or software receive buffer. */
void Usart1_Init(void);

/* Returns Tx_NOK if TXE is not ready; Tx_OK after the byte has left the pin. */
uint8 Usart1_TransmitByte(uint8 Byte);
void Usart1_TransmitString(const char *Str);

/* Waits for a received byte. Keep the section-8 spelling for compatibility. */
uint8 Usart1_RecieveByte(void);

#endif
