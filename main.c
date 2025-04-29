#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "utils/display/display.h"

void copy_string(char *src, char *dst);

Graphics_Context g_sContext;

int main(void)
{
    // Stop watchdog timer
    WDT_A_holdTimer();


    char list[20][20];

    sprintf(list[0], "mario");
    sprintf(list[1], "gianni");
    sprintf(list[2], "tony");

    init_display(&g_sContext);
    display_information(10.0, &g_sContext);
    display_list(list, &g_sContext);



    // Loop forever
    while (1);
}

void copy_string(char *dst, char *src){
    int i = 0;

    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }

    // Null terminate destination string
    dst[i] = '\0';
}
