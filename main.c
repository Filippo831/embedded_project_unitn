#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

#include "utils/display/display.h"
#include "utils/controls/controls.h"
#include "utils/serial/serial.h"
#include "utils/serial/printf.h"


/*
 * - create some checks to verify the list length and the movement to make sure every movement stays in the boundries
 * - try to find a way to optimize the display visualization becasue refreshing every time is time consuming
 * - test serial comunication
 */




Graphics_Context g_sContext;


// list of action and its index
char list[20][20];
uint8_t index = 0;


// variable for temperature reading
uint16_t cal30;
uint16_t cal85;
float calDifference;
float temperature;

uint16_t counter = 0;


uint8_t RXData;


/*
 * indicates whether the cursor is in "center" mode or in "point" mode
 * 0 -> center
 * 1 -> point
 */
static uint8_t cursorStatus = 0;

char word[20];
static uint8_t wordIndex = 0;

// keeps track of the cursor position with values from 0 -> down, to 2^16 -> up
static uint16_t cursorPosition;

int main(void)
{
    /* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // first init the display
    init_display(&g_sContext);

    // init temperature sensor
    REF_A_enableTempSensor();
    REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    REF_A_enableReferenceVoltage();

    cal30 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_30_DEGREES_C);
    cal85 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_85_DEGREES_C);
    calDifference = cal30 - cal85;


    // then init the adc converter otherwise it does not work
    setup_adc();
    setup_serial();


    sprintf(list[0], "mario");
    sprintf(list[1], "gianni");
    sprintf(list[2], "tony");
    sprintf(list[3], "antonello");
    sprintf(list[4], "antonello");
    sprintf(list[5], "mariuccio");
    sprintf(list[6], "marilena");
    sprintf(list[7], "giovanna");


    get_list(list);
    display_list(&g_sContext);

    // Loop forever
    while (1) {
        MAP_PCM_gotoLPM0();

    }
}

void ADC14_IRQHandler(void)
{
    uint64_t status;

    int16_t conRes;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1)
    {
        /* Store ADC14 conversion results */
        cursorPosition = ADC14_getResult(ADC_MEM1);

        /*
         * cursorStatus = used to avoid repeated action when selecting due to conversion noise. (sometimes instead of one step it took 2 steps)
         * - if cursor status is in center mode, check if the cursor goes to one of the 2 borders and perform the related function
         * - set cursorStatus to 1 to avoid repeating the action
         * - when cursorStatus == 1 check if the cursor comes back to a more centric position. The boundries are shifted towards the center
         * to avoid noise issues
         */

        if (cursorStatus == 0){
            if (cursorPosition < 1000) {
                scroll_down(&g_sContext);
                cursorStatus = 1;
            } else if (cursorPosition > 15000) {
                scroll_up(&g_sContext);
                cursorStatus = 1;
            }
        } else {
            if ((cursorPosition > 1500) && (cursorPosition < 15000)) {
                cursorStatus = 0;
            }
        }

        /*
        conRes = ((ADC14_getResult(ADC_MEM0) - cal30) * 55);
        temperature = (conRes / calDifference) + 30.0f;

        display_information(temperature, &g_sContext);
        */


        /*
        if (counter == 0) {
            conRes = ((ADC14_getResult(ADC_MEM0) - cal30) * 55);
            temperature = (conRes / calDifference) + 30.0f;

            display_information(temperature, &g_sContext);

            counter = 500;
        }
        counter = counter - 1;
        */
    }
}

void PORT5_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    if (status & GPIO_PIN1) {
        scroll_up(&g_sContext);
    }
}


void EUSCIA0_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        MAP_UART_transmitData(EUSCI_A0_BASE, MAP_UART_receiveData(EUSCI_A0_BASE));
    }
    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);
    return;
}

