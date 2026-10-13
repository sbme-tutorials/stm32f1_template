#include "Rcc.h"
#include "Rcc_Private.h"
#include "Bit_Math.h"

void Rcc_Init(void)
{
    SET_BIT(RCC_CR, 0); /* HSION */
    while (READ_BIT(RCC_CR, 1) == 0U) { } /* HSIRDY */

    RCC_CFGR &= ~0x3UL; /* SW=00: select HSI */
    while ((RCC_CFGR & 0xCUL) != 0U) { } /* SWS=00: HSI selected */
    /* HPRE=0000, PPRE1=000, PPRE2=000: core and both APB buses = 8 MHz. */
    RCC_CFGR &= ~((0xFUL << 4U) | (0x7UL << 8U) | (0x7UL << 11U));
}

void Rcc_Enable(uint8 PeripheralId)
{
    uint8 BusId = PeripheralId / 32U;
    uint8 PeripheralBitPosition = PeripheralId % 32U;

    switch (BusId) {
        case RCC_AHB:
            SET_BIT(RCC_AHBENR, PeripheralBitPosition);
            (void)RCC_AHBENR;
            break;
        case RCC_APB1:
            SET_BIT(RCC_APB1ENR, PeripheralBitPosition);
            (void)RCC_APB1ENR;
            break;
        case RCC_APB2:
            SET_BIT(RCC_APB2ENR, PeripheralBitPosition);
            (void)RCC_APB2ENR;
            break;
        default:
            break;
    }
}

void Rcc_Disable(uint8 PeripheralId)
{
    uint8 BusId = PeripheralId / 32U;
    uint8 PeripheralBitPosition = PeripheralId % 32U;

    switch (BusId) {
        case RCC_AHB:
            CLEAR_BIT(RCC_AHBENR, PeripheralBitPosition);
            break;
        case RCC_APB1:
            CLEAR_BIT(RCC_APB1ENR, PeripheralBitPosition);
            break;
        case RCC_APB2:
            CLEAR_BIT(RCC_APB2ENR, PeripheralBitPosition);
            break;
        default:
            break;
    }
}
