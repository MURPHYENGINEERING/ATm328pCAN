#ifndef __ATM328P_SPI_H__
#define __ATM328P_SPI_H__

#include "types.h"


/* Power Reduction Register */
typedef union {
    struct {
        VBOOL_T Unused0 : 1;
        VBOOL_T Unused1 : 1;
        /* Power Reduction Serial Peripheral Interface */
        VBOOL_T PRSPI : 1;
        VBOOL_T Unused3 : 1;
        VBOOL_T Unused4 : 1;
        VBOOL_T Unused5 : 1;
        VBOOL_T Unused6 : 1;
        VBOOL_T Unused7 : 1;
    } bits;
    VU8_T byte;
} PRR_T;

extern volatile PRR_T PRR;

#define PRSPI_DISABLE_POWER_REDUCTION FALSE
#define PRSPI_ENABLE_POWER_REDUCTION TRUE



/* SPI Control Register */
typedef union {
    struct {
        /* SPI Clock Rate Select */
        /* 0, 0 = fosc/4 
         * 0, 1 = fosc/16
         * 1, 0 = fosc/64
         * 1, 1 = fosc/128 */
        VBOOL_T SPR0 : 1;
        VBOOL_T SPR1 : 1;
        /* Clock Phase */
        /* When written to 1, SETUP is on the leading edge. 
         * When 0, SAMPLE is on the leading edge. */
        VBOOL_T CPHA : 1;
        /* Clock Polarity */
        /* When written to 1, SCK is HIGH when idle. When 0, SCK is LOW when idle. */
        VBOOL_T CPOL : 1;
        /* Master/Slave Select */
        VBOOL_T MSTR : 1;
        /* Data Order */
        VBOOL_T DORD : 1;
        /* SPI Enable */
        VBOOL_T SPE : 1;
        /* SPI Interrupt Enable */
        VBOOL_T SPIE : 1;
    } bits;
    VU8_T byte;
} SPCR_T;

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
#define CPHA_SAMPLE_ON_LEADING_EDGE FALSE
#define CPHA_SETUP_ON_LEADING_EDGE TRUE

#define SPCR_SPR0 0
#define SPCR_SPR1 1
#define SPCR_PRESCALE_OVER_128 (U8_T) ( (1u << SPCR_SPR1) | (1u << SPCR_SPR0) )


/* SPI Status Register */
typedef union {
    struct {
        /* Double SPI Speed Bit */
        /* When set, the clock speed selected by SPR1, SPR0 will be doubled. 
         * I.e., fosc/2, 8, 32, or 64 */
        VBOOL_T SPI2X : 1;
        VBOOL_T Unused1 : 1;
        VBOOL_T Unused2 : 1;
        VBOOL_T Unused3 : 1;
        VBOOL_T Unused4 : 1;
        VBOOL_T Unused5 : 1;
        /* Write Collision Flag */
        VBOOL_T WCOL : 1;
        /* SPI Interrupt Flag */
        VBOOL_T SPIF : 1;
    } bits;
    VU8_T byte;
} SPSR_T;

extern volatile SPSR_T SPSR;


/* SPI Data Register */
extern volatile REGISTER_T SPDR;


/* PORTB */
typedef union {
    struct {
        VBOOL_T Unused0 : 1;
        VBOOL_T Unused1 : 1;
        /* SPI bus master slave select */
        VBOOL_T SS : 1;
        /* SPI bus Master Out Slave In */
        VBOOL_T MOSI : 1;
        /* SPI bus Master In Slave Out */
        VBOOL_T MISO : 1;
        /* SPI bus master clock */
        VBOOL_T SCK : 1;
        VBOOL_T Unused6 : 1;
        VBOOL_T Unused7 : 1;
    } bits;
    VU8_T byte;
} REGISTER_SPI_T;

/* DDRB */
extern volatile REGISTER_SPI_T DDR_SPI;
/* PORTB */
extern volatile REGISTER_SPI_T PORT_SPI;
/* PINB */
extern volatile REGISTER_SPI_T PIN_SPI;


#endif