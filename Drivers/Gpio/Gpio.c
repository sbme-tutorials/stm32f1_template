#include "Gpio.h"
#include "Gpio_Private.h"

static GpioType *const addressMap[4] = {
    (GpioType *)GPIOA_BASE_ADDR,
    (GpioType *)GPIOB_BASE_ADDR,
    (GpioType *)GPIOC_BASE_ADDR,
    (GpioType *)GPIOD_BASE_ADDR
};

void Gpio_Init(uint8 PortName, uint8 PinNumber, uint8 PinMode, uint8 DefaultState)
{
    if (PortName < GPIO_A || PortName > GPIO_D || PinNumber > 15U) {
        return;
    }

    GpioType *gpioDevice = addressMap[PortName - GPIO_A];
    uint32 configuration;

    switch (PinMode) {
        case GPIO_INPUT:
            if (DefaultState == GPIO_NO_PULL_DOWN) {
                configuration = 0x4U; /* CNF=01, MODE=00: floating input */
            } else if (DefaultState == GPIO_PULL_UP || DefaultState == GPIO_PULL_DOWN) {
                /* With CNF=10/MODE=00, ODR selects the pull direction. */
                gpioDevice->GPIO_BSRR = DefaultState == GPIO_PULL_UP
                        ? (1UL << PinNumber) : (1UL << (PinNumber + 16U));
                configuration = 0x8U;
            } else {
                return;
            }
            break;
        case GPIO_OUTPUT:
            if (DefaultState > GPIO_OPEN_DRAIN) {
                return;
            }
            configuration = 0x2U | ((uint32)DefaultState << 2U);
            break;
        case GPIO_AF:
            if (DefaultState > GPIO_OPEN_DRAIN) {
                return;
            }
            configuration = 0xAU | ((uint32)DefaultState << 2U);
            break;
        case GPIO_ANALOG:
            configuration = 0x0U;
            break;
        default:
            return;
    }

    volatile uint32 *configurationRegister = PinNumber < 8U
            ? &gpioDevice->GPIO_CRL : &gpioDevice->GPIO_CRH;
    uint8 shift = (PinNumber % 8U) * 4U;
    *configurationRegister = (*configurationRegister & ~(0xFUL << shift))
            | (configuration << shift);
}

uint8 Gpio_WritePin(uint8 PortName, uint8 PinNumber, uint8 Data)
{
    if (PortName < GPIO_A || PortName > GPIO_D || PinNumber > 15U || Data > HIGH) {
        return NOK;
    }

    GpioType *gpioDevice = addressMap[PortName - GPIO_A];
    uint32 configuration = PinNumber < 8U ? gpioDevice->GPIO_CRL : gpioDevice->GPIO_CRH;
    uint8 shift = (PinNumber % 8U) * 4U;
    if (((configuration >> shift) & 0x3U) == 0U) {
        return NOK; /* MODE=00: input or analog, not output */
    }

    gpioDevice->GPIO_BSRR = Data == HIGH
            ? (1UL << PinNumber) : (1UL << (PinNumber + 16U));
    return OK;
}

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNumber)
{
    if (PortName < GPIO_A || PortName > GPIO_D || PinNumber > 15U) {
        return LOW;
    }
    GpioType *gpioDevice = addressMap[PortName - GPIO_A];
    return (uint8)((gpioDevice->GPIO_IDR >> PinNumber) & 1U);
}

uint8 Gpio_TogglePin(uint8 PortName, uint8 PinNumber)
{
    if (PortName < GPIO_A || PortName > GPIO_D || PinNumber > 15U) {
        return NOK;
    }
    GpioType *gpioDevice = addressMap[PortName - GPIO_A];
    uint8 nextState = ((gpioDevice->GPIO_ODR >> PinNumber) & 1U) == HIGH ? LOW : HIGH;
    return Gpio_WritePin(PortName, PinNumber, nextState);
}
