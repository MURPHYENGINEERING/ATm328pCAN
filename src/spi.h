#ifndef __SPI_H__
#define __SPI_H__

#include "types.h"

/** Enumerates the Master/Slave modes for the SPI peripheral device. */
typedef enum {
    SPI_MODE_MASTER,
    SPI_MODE_SLAVE
} SPI_MODE_T;


/** Enumerates the Byte Endian modes for the SPI peripheral device. */
typedef enum {
    SPI_ENDIAN_MSB_FIRST,
    SPI_ENDIAN_LSB_FIRST
} SPI_ENDIAN_T;


/** Enumerates the Clock Polarity modes for the SPI peripheral device. */
typedef enum {
    SPI_POLARITY_LEADING_IS_RISING,
    SPI_POLARITY_LEADING_IS_FALLING
} SPI_POLARITY_T;


/** Enumerates the Phase modes for the SPI peripheral device. */
typedef enum {
    SPI_PHASE_SAMPLE_ON_LEADING,
    SPI_PHASE_SETUP_ON_LEADING
} SPI_PHASE_T;


/** Enumerates the SPI prescaler divisors. */
typedef enum {
    SPI_PRESCALE_OVER_256
} SPI_PRESCALE_T;


/** Configures the SPI peripheral device on init. */
typedef struct {
    /** Enable or disable the SPI peripheral device. */
    ENABLE_T enable;
    /** Select Master or Slave mode for SPI. */
    SPI_MODE_T mode;
    /** Transmit data MSB or LSB first. */
    SPI_ENDIAN_T endian;
    /** Select whether the leading edge is rising or falling. */
    SPI_POLARITY_T polarity;
    /** Select whether the leading edge is setup or sampling. */
    SPI_PHASE_T phase;
    /** Set the SPI prescaler. */
    SPI_PRESCALE_T prescale;
    /** Enable or disable SPI "data byte ready" interrupts. */
    ENABLE_T interrupts;
} SPI_CFG_T;


void spi_init(SPI_CFG_T cfg);

void spi_begin(void);
void spi_end(void);

U8_T spi_tx_rx(U8_T tx_data);

#endif