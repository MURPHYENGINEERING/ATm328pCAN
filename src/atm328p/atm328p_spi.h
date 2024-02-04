#ifndef __ATM328P_SPI_H__
#define __ATM328P_SPI_H__

#include "types.h"


/** SPI Control Register */
typedef union {
    struct {
        /** SPI Clock Rate Select */
        VBOOL_T SPR0    : 1;
        VBOOL_T SPR1    : 1;
        /** Clock Phase */
        VBOOL_T CPHA    : 1;
        /** Clock Polarity */
        VBOOL_T CPOL    : 1;
        /** Master/Slave Select */
        VBOOL_T MSTR    : 1;
        /** Data Order */
        VBOOL_T DORD    : 1;
        /** SPI Enable */
        VBOOL_T SPE     : 1;
        /** SPI Interrupt Enable */
        VBOOL_T SPIE    : 1;
    } bits;
    VU8_T byte;
} SPCR_T;
/** SPI Control Register */
extern volatile SPCR_T SPCR;

#define SPE_DISABLE_SPI FALSE
#define SPE_ENABLE_SPI TRUE
#define SPIE_DISABLE_SPI_INTERRUPT FALSE
#define SPIE_ENABLE_SPI_INTERRUPT TRUE
#define DORD_MSB_FIRST FALSE
#define DORD_LSB_FIRST TRUE
#define MSTR_MODE_SLAVE FALSE
#define MSTR_MODE_MASTER TRUE
#define CPOL_LEADING_EDGE_IS_RISING FALSE
#define CPOL_LEADING_EDGE_IS_FALLING TRUE
#define CPHA_SAMPLE_ON_LEADING_EDGE FALSE
#define CPHA_SETUP_ON_LEADING_EDGE TRUE

#define SPCR_SPR0 0
#define SPCR_SPR1 1
#define SPCR_PRESCALE_OVER_128 (U8_T) ( (1u << SPCR_SPR1) | (1u << SPCR_SPR0) )


/** SPI Status Register */
typedef union {
    struct {
        /** Double SPI Speed Bit */
        VBOOL_T SPI2X   : 1;
        VBOOL_T Unused1 : 1;
        VBOOL_T Unused2 : 1;
        VBOOL_T Unused3 : 1;
        VBOOL_T Unused4 : 1;
        VBOOL_T Unused5 : 1;
        /** Write Collision Flag */
        VBOOL_T WCOL    : 1;
        /** SPI Interrupt Flag */
        VBOOL_T SPIF    : 1;
    } bits;
    VU8_T byte;
} SPSR_T;
/** SPI Status Register */
extern volatile SPSR_T SPSR;


/** SPI Data Register */
extern volatile REGISTER_T SPDR;


/** SPI GPIO register */
typedef union {
    struct {
        VBOOL_T Unused0 : 1;
        VBOOL_T Unused1 : 1;
        /** SPI bus master slave select */
        VBOOL_T SS      : 1;
        /** SPI bus Master Out Slave In */
        VBOOL_T MOSI    : 1;
        /** SPI bus Master In Slave Out */
        VBOOL_T MISO    : 1;
        /** SPI bus master clock */
        VBOOL_T SCK     : 1;
        VBOOL_T Unused6 : 1;
        VBOOL_T Unused7 : 1;
    } bits;
    VU8_T byte;
} REGISTER_SPI_T;

/** Data Direction Register for SPI (DDRB) */
extern volatile REGISTER_SPI_T DDR_SPI;
/** Port Register for SPI (PORTB) */
extern volatile REGISTER_SPI_T PORT_SPI;
/** Pin Register for SPI (PINB) */
extern volatile REGISTER_SPI_T PIN_SPI;


#endif