#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "utils/display/display.h"


/*
 * - create some checks to verify the list length and the movement to make sure every movement stays in the boundries
 * - try to find a way to optimize the display visualization becasue refreshing every time is time consuming
 * - test serial comunication
 */




void setup_timer();
static int timerClock = 0;

Graphics_Context g_sContext;
char list[20][20];
int index = 0;

int main(void)
{
    // Stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;


    sprintf(list[0], "mario");
    sprintf(list[1], "gianni");
    sprintf(list[2], "tony");

    init_display(&g_sContext);
    display_information(10.0, &g_sContext);
    display_list(list, &g_sContext);

    P1->DIR |= BIT0;
    P1->OUT |= BIT0;

    setup_timer();
    // Loop forever
    while (1) {
        __sleep();
        if (timerClock == 1) {
            P1->OUT ^= BIT0;
            timerClock = 0;
            select_index(index, list, &g_sContext);
            index = (index + 1) % 3;
        }
    }
}

void setup_timer() {
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCR[0] = 5000;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__ACLK | TIMER_A_CTL_MC__CONTINUOUS | TIMER_A_CTL_ID_1;
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
}



void TA0_0_IRQHandler() {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;

    timerClock = 1;

}

