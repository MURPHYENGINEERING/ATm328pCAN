#include "adc.h"

#define ADC_RESULT_T_MAX 1023
#define ADC_VREF 5.0f


FLOAT_T adc_to_volts(ADC_RESULT_T result)
{
    FLOAT_T v;

    v = ADC_VREF * ( (FLOAT_T) result / (FLOAT_T) ADC_RESULT_T_MAX );

    return v;
}