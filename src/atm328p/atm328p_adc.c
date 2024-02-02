#include "atm328p_adc.h"
#include "atm328p_mcu.h"
#include "atm328p_dsc.h"
#include "fai.h"
#include "adc.h"
#include "interrupts.h"
#include "memory.h"


/** Current mode the ADC was initialized to (blocking or non-blocking). */
ADC_MODE_T g_adc_mode;
/** Last conversion result from the ADC. */
volatile ADC_RESULT_T g_adc_result;


static inline void adc_sample_internal(void) __attribute__((always_inline));


/*******************************************************************************
 * In non-blocking mode, the ADC triggers this interrupt when a conversion is
 * completed.
 ******************************************************************************/
ISR(ADC_vect)
{
    adc_sample_internal();
}


/*******************************************************************************
 * Initializes the ADC hardware by disabling power reduction and configuring
 * the prescaler, interrupts, and auto trigger based on the specified mode.
 * \param[in] mode  The mode to configure the ADC in. Blocking means the ADC
 *                  will start a conversion on sample and wait until the
 *                  conversion finishes before returning. Non-blocking means the
 *                  ADC is constantly converting and sampling returns the last
 *                  conversion result.
 ******************************************************************************/
void adc_init(ADC_MODE_T mode)
{
    g_adc_mode = mode;

    PRR.bits.PRADC = PRADC_ENABLE_ADC;

    /* Start from scratch so the ADC can be reinitialized/reconfigured. */
    ADMUX.byte = (U8_T) 0;
    ADCSRA.byte = (U8_T) 0;
    ADCSRB.byte = (U8_T) 0;

    /* The ADC doesn't work at any prescaler faster than this. */
    ADCSRA.byte |= ADPS_PRESCALE_OVER_4;

    /* Select the appropriate reference for the Arduino UNO development board
     * (AVcc with external capacitor at AREF pin. */
    ADMUX.byte |= REFS_AVCC;
    /* The low bits of the result will be in the low bits of the data register. */
    ADMUX.bits.ADLAR = ADLAR_RIGHT_ADJUST;

    /* Default to the first ADC. */
    adc_select(ADC_0);

    if (ADC_MODE_NONBLOCKING == mode) {
        ADCSRB.byte |= ADTS_FREE_RUNNING;
        ADCSRA.bits.ADIE = ADIE_ENABLE_CONVERSION_INTERRUPT;
        ADCSRA.bits.ADATE = ADATE_ENABLE_AUTO_TRIGGER;
    } else {
        ADCSRA.bits.ADATE = ADATE_DISABLE_AUTO_TRIGGER;
        ADCSRA.bits.ADIE = ADIE_DISABLE_CONVERSION_INTERRUPT;
    }

    ADCSRA.bits.ADEN = ADEN_ENABLE_ADC;
    /* Start the first conversion if in free-running mode. */
    ADCSRA.bits.ADSC = ADSC_CONVERTING;
}


/*******************************************************************************
 * Sample the ADC, either starting a conversion and waiting (Blocking mode) or 
 * retrieving the last conversion result (Non-blocking mode).
 * \return The ADC conversion result.
 ******************************************************************************/
ADC_RESULT_T adc_sample(void)
{
    if (ADC_MODE_BLOCKING == g_adc_mode) {
        /* Start conersion */
        ADCSRA.bits.ADSC = ADSC_CONVERTING;
        /* Wait for conversion to finish */
        while (ADSC_CONVERTING == ADCSRA.bits.ADSC) {
        }
        
        adc_sample_internal();
    }
    
    return g_adc_result;
}


/*******************************************************************************
 * Sample the hardware by reading from the ADC data register.
 ******************************************************************************/
static inline void adc_sample_internal(void)
{
    /* "... ADCL must be read first, then ADCH."
     * - ATmega328P Datasheet page 219 */
    g_adc_result = (ADC_RESULT_T)( ADC.bytes.low.byte & ADCL_MASK );
    g_adc_result |= (ADC_RESULT_T)( (ADC.bytes.high.byte & ADCH_MASK) << 8 );
}


/*******************************************************************************
 * Select the given ADC for conversion by setting the appropriate MUX register.
 * This function reports a software error fault if the given ADC is out of range.
 * \param[in] adc The specified ADC to be activated. 
 ******************************************************************************/
void adc_select(ADC_T adc)
{
    ADMUX.byte &= ~ADMUX_MUX_MASK;

    switch (adc) {
    case ADC_0:             ADMUX.byte |= ADMUX_ADC0;           break;
    case ADC_1:             ADMUX.byte |= ADMUX_ADC1;           break;
    case ADC_2:             ADMUX.byte |= ADMUX_ADC2;           break;
    case ADC_3:             ADMUX.byte |= ADMUX_ADC3;           break;
    case ADC_4:             ADMUX.byte |= ADMUX_ADC4;           break;
    case ADC_5:             ADMUX.byte |= ADMUX_ADC5;           break;
    case ADC_6:             ADMUX.byte |= ADMUX_ADC6;           break;
    case ADC_7:             ADMUX.byte |= ADMUX_ADC7;           break;
    case ADC_TEMPERATURE:   
        ADMUX.byte |= ADMUX_TEMPERATURE;
        /* The internal 1.1V voltage reference must also be selected for the ADC 
         * voltage reference source in the temperature sensor measurement.
         * - ATmega328P Datasheet page 215 */
         ADMUX.byte &= ~REFS_MASK;
         ADMUX.byte |= REFS_INTERNAL_1V1;
    break;
    case ADC_1V1:           ADMUX.byte |= ADMUX_1V1;            break;
    case ADC_GND:           ADMUX.byte |= ADMUX_GND;            break;

    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
        break;
    }
}


