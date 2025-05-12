#include "serial.h"

const eUSCI_UART_ConfigV1 uartConfigAclk =
{
    EUSCI_A_UART_CLOCKSOURCE_ACLK, // ACLK Clock Source
    3,                          // clockPrescalar
    0,                          // firstModReg
    146,                        // secondModReg
    EUSCI_A_UART_NO_PARITY,
    EUSCI_A_UART_LSB_FIRST,
    EUSCI_A_UART_ONE_STOP_BIT,
    EUSCI_A_UART_MODE,
    EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION
};


void setup_serial(void)
{
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3,
            GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfigAclk);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

    // Interrupt enable Master must be called: MAP_Interrupt_enableMaster();
    lprintf(EUSCI_A0_BASE, "PRINT> Initialized.\r\n");
}
