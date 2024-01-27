#include "types.h"
#include "atm328p.h"


ISR(TIMER1_COMPA_vect)
{
    PORT_BUILTIN.bits.LED = !PORT_BUILTIN.bits.LED;
    TCNT1.halfword = (U16_T) 0;
}


ISR(SPI_STC_vect)
{
}


S32_T main()
{
    DDR_BUILTIN.bits.LED = 1;

    /* Normal timer mode */
    TCCR1A.byte = (U8_T) TCCR1A_NORMAL_MODE;

    /* Enable prescaler (clk / 256) */
    TCCR1B.bits.CS12 = 1;

    /* Enable timer overflow interrupt */
    /* TIMSK1.bits.TOIE1 = 1; */

    /* Enable COMP A interrupt */
    TIMSK1.bits.OCIE1A = OCIE1A_ENABLE_INTERRUPT;
    OCR1A.halfword = (U16_T) 0x0AFFu;

    /* Turn off Power Reduction for SPI */
    PRR.bits.PRSPI = PRSPI_ENABLE_POWER_REDUCTION;

    /* Enable SPI in master mode with interrupts enabled, */
    SPCR.bits.SPE = SPE_ENABLE_SPI;
    SPCR.bits.SPIE = SPIE_ENABLE_SPI_INTERRUPT;
    SPCR.bits.MSTR = MSTR_MASTER_MODE;
    /* Sampled on the Falling edge, */
    SPCR.bits.CPOL = CPOL_LEADING_IS_RISING;
    SPCR.bits.CPHA = CPHA_SAMPLE_ON_LEADING;
    /* at clk/128. */
    SPCR.bits.SPR0 = 1;
    SPCR.bits.SPR1 = 1;

    /* Setup output and input pins */
    DDR_SPI.bits.SCK = DDR_OUTPUT;
    DDR_SPI.bits.MOSI = DDR_OUTPUT;
    DDR_SPI.bits.MISO = DDR_INPUT;
    DDR_SPI.bits.SS = DDR_OUTPUT;

    /* Enable interrupts */
    sei();

    /* Run forever in interrupt-driven mode */
    while(TRUE)
    {
    }
}