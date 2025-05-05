#include "controls.h"

void setup_adc() {

}

void init_adc() {
    /* Initializing ADC (ADCOSC/64/8) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_64, ADC_DIVIDER_8, ADC_TEMPSENSEMAP);

    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
}

void enable_interrupts() {
    //MAP_ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192, ADC_PULSE_WIDTH_192);

    MAP_ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();
}

void setup_cursor(){
    /* Configures Pin 6.0 as ADC input */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);


    MAP_ADC14_configureSingleSampleMode(ADC_MEM1, true);

    MAP_ADC14_configureConversionMemory(ADC_MEM1,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);
}


void setup_temperature() {

    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);

    MAP_ADC14_configureConversionMemory(ADC_MEM0,
         ADC_VREFPOS_INTBUF_VREFNEG_VSS,
         ADC_INPUT_A22, false);

    MAP_ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192, ADC_PULSE_WIDTH_192);


}
