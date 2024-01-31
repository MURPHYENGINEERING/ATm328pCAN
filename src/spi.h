#ifndef __SPI_H__
#define __SPI_H__

/******************************************************************************/
void spi_init(void);

void spi_activate(void);
void spi_deactivate(void);

U8_T spi_tx_rx(U8_T tx_data);
/******************************************************************************/

extern VBOOL_T g_spi_ready;

#endif