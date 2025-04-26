#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "utils/display/display.h"


Graphics_Context g_sContext;

int main(void)
{
    // Stop watchdog timer
    WDT_A_holdTimer();

    char* list[20] = {"mario", "tony", "gianni", "andrea", "alberto"};
    init_display();
    display_information(10);
    display_list(list);



    // Loop forever
    while (1);
}

