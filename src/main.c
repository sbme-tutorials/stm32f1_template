#include "Rcc.h"
#include "Usart.h"
#include "board.h"

int main(void)
{
    Rcc_Init();
    Panda_DisableBuzzer();
    Usart1_Init();

    while (1) {
        /* Wait for an ASCII digit; ignore Enter/CR/LF and other characters. */
        switch (Usart1_RecieveByte()) {
            case '1':
                Usart1_TransmitString("Hello\r\n");
                break;
            case '2':
                Usart1_TransmitString("Welcome\r\n");
                break;
            default:
                break;
        }
    }
}
