#include "controls.h"

void test() {
    // Enable ADC module
    MAP_ADC14_enableModule();

    // Initialize ADC14 with MCLK source (assumes default 3 MHz MCLK)
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK,
                         ADC_PREDIVIDER_1,
                         ADC_DIVIDER_1,
                         0);

    // Configure P6.0 (A15) as analog input for cursor
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    // Set 14-bit resolution
    MAP_ADC14_setResolution(ADC_14BIT);

    // Configure sequence: MEM0 = A15 (cursor), MEM1 = internal temp sensor
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    MAP_ADC14_configureConversionMemory(ADC_MEM1,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A9,
                                        false);

    MAP_ADC14_configureConversionMemory(ADC_MEM0,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A22,
                                        false);

    // Enable internal reference for temperature sensor
    MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    MAP_REF_A_enableReferenceVoltage();
    MAP_ADC14_enableReferenceBurst();

    // Enable interrupt on MEM1 (end of sequence)
    MAP_ADC14_enableInterrupt(ADC_INT1);
    MAP_Interrupt_enableInterrupt(INT_ADC14);

    // Enable ADC conversions
    MAP_ADC14_enableConversion();

    // Use automatic sample timer to repeat sequence
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    MAP_ADC14_toggleConversionTrigger();  // Start conversion

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

void setup_button() {

    // Set P5.1 as input with pull-up resistor
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);

    // Enable interrupt on P5.1 (falling edge when button pressed)
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);

    // Enable PORT5 interrupt in NVIC
    Interrupt_enableInterrupt(INT_PORT5);

}
