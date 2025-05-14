#include "controls.h"

void setup_adc() {
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, ADC_TEMPSENSEMAP);

    // Configure Memory:
    // MEM0 -> A22 (Temperature Sensor)
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A22, false);

    // MEM1 -> A15 (Assuming joystick X-axis is connected to P6.0)
    ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9, false);

    // Enable sequence-of-channels mode (for multiple MEM readings)
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192, ADC_PULSE_WIDTH_192);
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    ADC14_enableInterrupt(ADC_INT1);  // Use interrupt on MEM1 (last in sequence)

    Interrupt_enableInterrupt(INT_ADC14);


    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();

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
