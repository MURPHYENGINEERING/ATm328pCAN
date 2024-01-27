#include "types.h"
#include "atm328p.h"
#include "memory.h"
#include "spi.h"


U8_T g_spi_fifo_item_len;

ISR(TIMER1_OVF_vect)
{
    /*
    PORT_CANBOARD.bits.LED1 = !PORT_CANBOARD.bits.LED1;
    PORT_CANBOARD.bits.LED2 = !PORT_CANBOARD.bits.LED2;
    */
    /*
    TCNT1.halfword = (U16_T) 0;
    */
   spi_tx_q_add((U8_T*) "Hello, world!", 13);
}


ISR(SPI_STC_vect)
{
}



S32_T main(void)
{
    g_spi_fifo_item_len = 0;

    DDR_CANBOARD.bits.LED1 = DDR_OUTPUT;
    DDR_CANBOARD.bits.LED2 = DDR_OUTPUT;
    PORT_CANBOARD.bits.LED1 = LOW;
    PORT_CANBOARD.bits.LED2 = LOW;
    /* PORT_CANBOARD.bits.LED2 = HIGH; */

    /* Normal timer mode */
    TCCR1A.byte = TCCR1A_NORMAL_MODE;

    /* Enable prescaler (clk / 256) */
    TCCR1B.byte |= TCCR1B_PRESCALE_OVER_256;

    /* Enable timer overflow interrupt */
    TIMSK1.bits.TOIE1 = TOIE1_ENABLE_INTERRUPT;

    /* Enable COMP A interrupt */
    /*
    TIMSK1.bits.OCIE1A = OCIE1A_ENABLE_INTERRUPT;
    OCR1A.halfword = (U16_T) 0x0AFFu;
    */

    /* Turn off Power Reduction for SPI */
    PRR.bits.PRSPI = PRSPI_DISABLE_POWER_REDUCTION;

    /* Enable SPI in master mode with interrupts enabled, */
    SPCR.bits.SPE = SPE_ENABLE_SPI;
    SPCR.bits.SPIE = SPIE_ENABLE_SPI_INTERRUPT;
    SPCR.bits.MSTR = MSTR_MASTER_MODE;
    /* Sampled on the Falling edge, */
    SPCR.bits.CPOL = CPOL_LEADING_IS_RISING;
    SPCR.bits.CPHA = CPHA_SAMPLE_ON_LEADING;
    /* at clk/128. */
    SPCR.byte |= SPCR_PRESCALE_OVER_128;

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