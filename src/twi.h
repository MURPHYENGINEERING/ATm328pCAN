#ifndef __TWI_H__
#define __TWI_H__

#include "types.h


/** Enumerates the prescaler divisors for the TWI peripheral device. */
typedef enum {
    /** Prescale the TWI clock by FCLK/1 (no prescaling). */
    TWI_PRESCALE_OVER_1,
    /** Prescale the TWI clock by FCLK/4. */
    TWI_PRESCALE_OVER_4,
    /** Prescale the TWI clock by FCLK/16. */
    TWI_PRESCALE_OVER_16,
    /** Prescale the TWI clock by FCLK/64. */
    TWI_PRESCALE_OVER_64
} TWI_PRESCALE_T;


/** Configures the TWI peripheral device. */
typedef struct {
    /** Configure the TWI prescaler. */
    TWI_PRESCALE_T prescale;
} TWI_CFG_T;


void twi_init_hardware(TWI_CFG_T cfg);
void twi_init(TWI_CFG_T cfg);

PASS_T twi_master_tx(U8_T addr, U8_T* data, SIZE_T len);
SIZE_T twi_master_rx(U8_T addr, U8_T* data, SIZE_T len);

#endif