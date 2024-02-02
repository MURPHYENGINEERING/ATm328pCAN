#ifndef __ADC_H__
#define __ADC_H__

#include "types.h"


/** Represents the different ADCs that may be sampled from. */
typedef enum {
    ADC_0,
    ADC_1,
    ADC_2,
    ADC_3,
    ADC_4,
    ADC_5,
    ADC_6,
    ADC_7,
    /** Selects the on-chip temperature sensor for reading the voltage across it. */
    ADC_TEMPERATURE,
    /** Selects the internal 1.1V voltage reference for reading. */
    ADC_1V1,
    /** Selects the internal 0V voltage reference for reading. */
    ADC_GND
} ADC_T;


/** Specifies the mode in which to configure the ADC. */
typedef enum {
    /** The ADC will start a conversion on sample and wait until the conversion
      * finishes before returning. */
    ADC_MODE_BLOCKING,
    /** The ADC will constantly convert, and sampling returns the last conversion
      * result. */
    ADC_MODE_NONBLOCKING
} ADC_MODE_T;


/** Represents a 10-bit result from the ADC. */
typedef U16_T ADC_RESULT_T;

void adc_init(ADC_MODE_T mode);
ADC_RESULT_T adc_sample(void);
void adc_select(ADC_T adc);

#endif