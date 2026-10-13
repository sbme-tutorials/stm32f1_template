#include "Utils.h"
#include "Rcc.h"

#define SYSTICK_CTRL (*(volatile uint32 *)0xE000E010UL)
#define SYSTICK_LOAD (*(volatile uint32 *)0xE000E014UL)
#define SYSTICK_VAL  (*(volatile uint32 *)0xE000E018UL)

void DelayMs(uint32 Milliseconds)
{
    SYSTICK_CTRL = 0U;
    SYSTICK_LOAD = RCC_HSI_FREQUENCY / 1000U - 1U; /* 7999: one millisecond */
    SYSTICK_VAL = 0U;
    SYSTICK_CTRL = 0x5U; /* ENABLE=1, TICKINT=0, CLKSOURCE=core clock */

    for (uint32 elapsed = 0U; elapsed < Milliseconds; ++elapsed) {
        while ((SYSTICK_CTRL & (1UL << 16U)) == 0U) { } /* COUNTFLAG */
    }
    SYSTICK_CTRL = 0U;
}
