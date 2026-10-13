/* Adapted from stm32f4-sec8/Usart/Usart.c for STM32F1 USART1. */
#include "Usart.h"
#include "Usart_Private.h"
#include "Gpio.h"
#include "Rcc.h"

void Usart1_Init(void)
{
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_USART1);

    /* F1 default USART1 mapping: TX is AF output; RX is an input, no AF7. */
    Gpio_Init(GPIO_A, USART1_TX_PIN, GPIO_AF, GPIO_PUSH_PULL);
    Gpio_Init(GPIO_A, USART1_RX_PIN, GPIO_INPUT, GPIO_PULL_UP);

    USART1->CR1 = 0U; /* Disable USART; 8 data bits, no parity or interrupts. */
    USART1->CR2 = 0U; /* Asynchronous mode, one stop bit. */
    USART1->CR3 = 0U; /* No hardware flow control or DMA. */

    /* Rcc_Init selects PCLK2 = 8 MHz. F103 oversampling is fixed at 16.
     * BRR encodes 16 * USARTDIV: round PCLK2 / baud to the nearest integer.
     * At 9600 baud this is 833 (0x341), not the F4 example's 0x683. */
    USART1->BRR = (RCC_HSI_FREQUENCY + USART1_BAUD_RATE / 2UL) / USART1_BAUD_RATE;

    /* Clear any old receive data/errors by reading SR followed by DR. */
    (void)USART1->SR;
    (void)USART1->DR;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

uint8 Usart1_TransmitByte(uint8 Byte)
{
    if ((USART1->SR & USART_SR_TXE) == 0U) {
        return Tx_NOK;
    }

    /* The SR read above followed by this DR write clears TC. */
    USART1->DR = Byte;
    while ((USART1->SR & USART_SR_TC) == 0U) { }
    return Tx_OK;
}

void Usart1_TransmitString(const char *Str)
{
    if (Str == NULL) {
        return;
    }

    while (*Str != '\0') {
        if (Usart1_TransmitByte((uint8)*Str) == Tx_OK) {
            ++Str;
        }
    }
}

uint8 Usart1_RecieveByte(void)
{
    while (1) {
        uint32 status = USART1->SR;
        if ((status & (USART_SR_RXNE | USART1_RECEIVE_ERROR_FLAGS | USART_SR_ORE)) != 0U) {
            /* SR then DR clears RXNE and error flags, including an overrun.
             * An overrun loses newer bytes; the pending byte can still be valid. */
            uint8 Byte = (uint8)USART1->DR;
            if ((status & USART_SR_RXNE) != 0U
                    && (status & USART1_RECEIVE_ERROR_FLAGS) == 0U) {
                return Byte;
            }
        }
    }
}
