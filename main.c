#include "msp.h"


/**
 * main.c
 */

void delay(uint32_t t);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer


	P1->DIR |= BIT0;
	P1->OUT &= ~BIT0;


	while(1) {
	    P1->OUT ^= BIT0;
	    delay(100000);
	}

}

void delay(uint32_t t) {
    volatile uint32_t i;
    for (i=0; i<t; i++);
}
