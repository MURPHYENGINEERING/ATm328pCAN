#include "types.h"
#include "spi.h"
#include "interrupts.h"


/******************************************************************************/
/* Master status flag */
VBOOL_T g_spi_ready;
/******************************************************************************/


/* Called when a SPI write finishes */
ISR(SPI_STC_vect)
{
    g_spi_ready = TRUE;
}