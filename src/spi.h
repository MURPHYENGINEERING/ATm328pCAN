#ifndef __SPI_H__
#define __SPI_H__

#include "types.h"

typedef enum {
    SPI_MODE_MASTER,
    SPI_MODE_SLAVE
} SPI_MODE_T;


typedef enum {
    SPI_ENDIAN_MSB_FIRST,
    SPI_ENDIAN_LSB_FIRST
} SPI_ENDIAN_T;


typedef enum {
    SPI_POLARITY_LEADING_IS_RISING,
    SPI_POLARITY_LEADING_IS_FALLING
} SPI_POLARITY_T;


typedef enum {
    SPI_PHASE_SAMPLE_ON_LEADING,
    SPI_PHASE_SETUP_ON_LEADING
} SPI_PHASE_T;


typedef enum {
    SPI_PRESCALE_OVER_256
} SPI_PRESCALE_T;


typedef struct {
    ENABLE_T enable;
    SPI_MODE_T mode;
    SPI_ENDIAN_T endian;
    SPI_POLARITY_T polarity;
    SPI_PHASE_T phase;
    SPI_PRESCALE_T prescale;
    ENABLE_T interrupts;
} SPI_CFG_T;


void spi_init(SPI_CFG_T cfg);

void spi_begin(void);
void spi_end(void);

U8_T spi_tx_rx(U8_T tx_data);

#endif