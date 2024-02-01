#include "types.h"
#include "spi.h"
#include "interrupts.h"


/* Master status flag */
VBOOL_T g_spi_ready;


/*******************************************************************************
 * Set the state of the SPI device to "ready."
 ******************************************************************************/
ISR(SPI_STC_vect)
{
    g_spi_ready = TRUE;
}