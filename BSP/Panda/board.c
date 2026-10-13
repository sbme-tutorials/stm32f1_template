#include "board.h"
#include "Rcc.h"
#include "stm32f1xx.h"

void Panda_DisableBuzzer(void)
{
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_AFIO);

    /* Prepare the low GPIO output before releasing PB4 from NJTRST. */
    Gpio_Init(PANDA_BUZZER_PORT, PANDA_BUZZER_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_WritePin(PANDA_BUZZER_PORT, PANDA_BUZZER_PIN, LOW);

    /* SWJ_CFG=001: free PB4 only; retain JTAG and SWD. These bits have
     * undefined readback, so replace the entire field explicitly. */
    AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_SWJ_CFG) | AFIO_MAPR_SWJ_CFG_NOJNTRST;
}
