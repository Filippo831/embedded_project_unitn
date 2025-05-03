#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

#include "utils/display/display.h"
#include "utils/controls/controls.h"


/*
 * - create some checks to verify the list length and the movement to make sure every movement stays in the boundries
 * - try to find a way to optimize the display visualization becasue refreshing every time is time consuming
 * - test serial comunication
 */




Graphics_Context g_sContext;
char list[20][20];
int index = 0;

static uint16_t cursorPosition[2];  // [X, Y]

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

    init_adc();

    //sprintf(list[0], "mario");
    //sprintf(list[1], "gianni");
    //sprintf(list[2], "tony");

    init_display(&g_sContext);
    //display_information(10.0, &g_sContext);
    //display_list(list, &g_sContext);

    // Loop forever
    while (1) {
        PCM_gotoLPM0();
    }
}


void ADC14_IRQHandler(void) {
    uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);


    if (status & ADC_INT1) {
        cursorPosition[0] = ADC14_getResult(ADC_MEM0);
        cursorPosition[1] = ADC14_getResult(ADC_MEM1);

        char string[10];
        sprintf(string, "X: %5d", cursorPosition[0]);
        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)string,
                                        8,
                                        64,
                                        50,
                                        OPAQUE_TEXT);

        sprintf(string, "Y: %5d", cursorPosition[1]);
        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)string,
                                        8,
                                        64,
                                        70,
                                        OPAQUE_TEXT);

        int buttonPressed = 0;
         if (!(P4IN & GPIO_PIN1))
             buttonPressed = 1;

         sprintf(string, "Button: %d", buttonPressed);
         Graphics_drawStringCentered(&g_sContext,
                                         (int8_t *)string,
                                         AUTO_STRING_LENGTH,
                                         64,
                                         90,
                                         OPAQUE_TEXT);
    }
}
