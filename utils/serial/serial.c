#include "serial.h"

const eUSCI_UART_ConfigV1 uartConfig =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,         // Clock source
    19,                                     // BRDIV = 19 (for 9600 baud @ 3MHz)
    8,                                      // UCxBRF
    85,                                     // UCxBRS
    EUSCI_A_UART_NO_PARITY,                 // No parity
    EUSCI_A_UART_LSB_FIRST,                 // LSB first
    EUSCI_A_UART_ONE_STOP_BIT,              // One stop bit
    EUSCI_A_UART_MODE,                      // UART mode
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};




void setup_serial() {
    /* Selecting P3.2 and P3.3 in UART mode and P1.0 as output (LED) */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
             GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    UART_initModule(EUSCI_A0_BASE, &uartConfig);
    UART_enableModule(EUSCI_A0_BASE);

}
