#ifndef __ATM328P_TWI_H__
#define __ATM328P_TWI_H__

#include "types.h"


/** TWI Control Register */
typedef union {
    struct {
        /** TWI Interrupt Enable bit (R/W) */
        VBOOL_T TWIE        : 1;
        VBOOL_T Reserved1   : 1;
        /** TWI Enable bit (R/W) */
        VBOOL_T TWEN        : 1;
        /** TWI Write Collision Flag (R) */
        VBOOL_T TWWC        : 1;
        /** TWI STOP Condition bit (R/W) */
        VBOOL_T TWSTO       : 1;
        /** TWI START Condition bit (R/W) */
        VBOOL_T TWSTA       : 1;
        /** TWI Enable Acknowledge bit (R/W) */
        VBOOL_T TWEA        : 1;
        /** TWI Interrupt Flag (R/W) */
        VBOOL_T TWINT       : 1;
    } bits;
    VU8_T byte;
} TWCR_T;
/** TWI Control Register */
extern volatile TWCR_T TWCR;

/** Position of the TWI Interrupt Flag bit in TWCR. */
#define TWCR_TWINT  7
/** Position of the TWI Enable Acknowledge bit in TWCR. */
#define TWCR_TWEA   6
/** Position of the TWI START Condition bit in TWCR. */
#define TWCR_TWSTA  5
/** Position of the TWI STOP Condition bit in TWCR. */
#define TWCR_TWSTO  4
/** Position of the TWI Enable bit in TWCR. */
#define TWCR_TWEN   2

/** Sets TWCR to indicate the START condition. */
#define TWCR_START_CONDITION \
    (U8_T)( (1u << TWCR_TWINT) | (1u << TWCR_TWSTA) | (1u << TWCR_TWEN) )
/** Sets TWCR to indicate the STOP condition. */
#define TWCR_STOP_CONDITION \
    (U8_T)( (1u << TWCR_TWINT) | (1u << TWCR_TWSTO) | (1u << TWCR_TWEN) )

/** Return an ACK in reponse to a data byte. */
#define TWCR_ACKNOWLEDGE_DATA \
    (U8_T)( (1u << TWCR_TWINT) | (1u << TWCR_TWEA) )
/** Return a NACK in response to a data byte. */
#define TWCR_ACKNOWLEDGE_DATA_END (U8_T)( (1u << TWCR_TWINT) )

/** Clears the TWINT flag and enables transmission. */
#define TWCR_TRANSMIT \
    (U8_T)( (1u << TWCR_TWINT) | (1u << TWCR_TWEN) )

/** Set TWINT to this value to clear the bit. */
#define TWINT_CLEAR_FLAG TRUE

/** TWI Bit Rate Register */
extern volatile REGISTER_T TWBR;


/** TWI Status Register */
typedef union {
    struct {
        /** TWI Bit Rate Prescaler bits (R/W) */
        VBOOL_T TWPS0       : 1;
        VBOOL_T TWPS1       : 1;
        VBOOL_T Reserved2   : 1;
        /** TWI Status bits (R) */
        VBOOL_T TWS3        : 1;
        VBOOL_T TWS4        : 1;
        VBOOL_T TWS5        : 1;
        VBOOL_T TWS6        : 1;
        VBOOL_T TWS7        : 1;
    } bits;
    VU8_T byte;
} TWSR_T;
/** TWI Status Register */
extern volatile TWSR_T TWSR;

/** Bit 0 of the TWI Bit Rate Prescaler register (TWSR). */
#define TWSR_TWPS0 0
/** Bit 1 of the TWI Bit Rate Prescaler Register (TWSR). */
#define TWSR_TWPS1 1

/** Select only the TWI Status bits in TWSR. */
#define TWSR_STATUS_MASK (U8_T) 0b11111000u

/** Select the TWI Status from the TWSR register. */
#define TWSR_STATUS (U8_T)( TWSR.byte & TWSR_STATUS_MASK )

/** TWSR Master Transmitter Mode */
#define TWS_MT_SLA_W        (U8_T) 0b00000000
/** TWSR Master Transmitter Slave Acknowledged status */
#define TWS_MT_SLA_ACK      (U8_T) 0x18u
/** TWSR Master Transmitter Data Acknowledged status */
#define TWS_MT_DATA_ACK     (U8_T) 0x28u

/** TWSR Master Receiver Mode */
#define TWS_MR_SLA_R        (U8_T) 0b00000001
/** TWSR Master Receiver Slave Acknowledged status */
#define TWS_MR_SLA_ACK      (U8_T) 0x40u
/** TWSR Master Receiver Data Acknowledged status */
#define TWS_MR_DATA_ACK     (U8_T) 0x50u

/** Set the TWI Bit Rate Prescaler register to FCLK/1. */
#define TWPS_PRESCALE_OVER_1 (U8_T) 0u
/** Set the TWI Bit Rate Prescaler register to FCLK/4. */
#define TWPS_PRESCALE_OVER_4 (U8_T)( 1u << TWSR_TWPS0 )
/** Set the TWI Bit Rate Prescaler register to FCLK/16. */
#define TWPS_PRESCALE_OVER_16 (U8_T) ( 1u << TWSR_TWPS1 )
/** Set the TWI Bit Rate Prescaler register to FCLK/64. */
#define TWPS_PRESCALE_OVER_64 (U8_T) ( (1u << TWSR_TWPS1) | (1u << TWSR_TWPS0) )


/** TWI Data Register */
extern volatile REGISTER_T TWDR;

/** Mark the given address as SLA+W. */
#define TWDR_SLAVE_WRITE(addr)  (U8_T)( addr | TWS_MR_SLA_W )
/** Mark the given address as SLA+R. */
#define TWDR_SLAVE_READ(addr)   (U8_T)( addr | TWS_MR_SLA_R )


/** TWI (Slave) Address Register */
typedef union {
    struct {
        /** TWI General Call Recognition Enable bit (R/W) */
        VBOOL_T TWGCE   : 1;
        /** TWI (Slave) Address Register bits (R/W) */
        VBOOL_T TWA0    : 1;
        VBOOL_T TWA1    : 1;
        VBOOL_T TWA2    : 1;
        VBOOL_T TWA3    : 1;
        VBOOL_T TWA4    : 1;
        VBOOL_T TWA5    : 1;
        VBOOL_T TWA6    : 1;
    } bits;
    VU8_T byte;
} TWAR_T;
/** TWI (Slave) Address Register */
extern volatile TWAR_T TWAR;


/** 7-bit (high bits) TWI (Slave) Address Mask Register */
extern volatile REGISTER_T TWAMR;


#endif