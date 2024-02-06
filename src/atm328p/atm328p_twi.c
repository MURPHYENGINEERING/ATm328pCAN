#include "atm328p_twi.h"
#include "twi.h"
#include "fai.h"
#include "memory.h"


/*******************************************************************************
 * Initialize the TWI peripheral device with the given configuration.
 * \param[in] cfg   The configuration to which the TWI peripheral device will be 
 *                  initialized.
 ******************************************************************************/
void twi_init_hardware(TWI_CFG_T cfg)
{
    TWCR.byte = (U8_T) 0u;
    TWSR.byte = (U8_T) 0u;

    switch (cfg.prescale) {
    case TWI_PRESCALE_OVER_1:
        TWSR.byte |= TWSR_PRESCALE_OVER_1;
        break;
    default:
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, (U32_T) get_pc());
        break;
    }
}


/*******************************************************************************
 * Transmit the given data to the given slave address on the TWI peripheral 
 * device.
 * \param[in] addr  The slave address to transmit to.
 * \param[in] data  Array of bytes to be transmitted in Master Transmitter mode.
 * \param[in] len   Length in bytes of the `data` array.
 * \return          `PASS` if the function completed successfully or `FAIL` if
 *                  there was a TWI error during transmission.
 ******************************************************************************/
PASS_T twi_master_tx(U8_T addr, U8_T* data, SIZE_T len)
{
    PASS_T status;
    SIZE_T i;

    status = PASS;

    TWCR.byte = TWCR_START_CONDITION;

    /* Wait for start condition to be set. */
    while (FALSE == TWCR.bits.TWINT) {
    }

    if (TWCR_START_CONDITION != TWSR_STATUS) {
        fai_pass_fail_logger(FAI_FAULT_ID_TWI_START, FAIL, (U32_T) TWSR_STATUS);
        status = FAIL;
    } else {
        /* Load the address with the Slave Write bit set and tansmit */
        TWDR.byte = TWDR_SLAVE_WRITE(addr);
        TWCR.byte = TWCR_TRANSMIT;

        /* Wait for address to be transmitted. */
        while (FALSE == TWCR.bits.TWINT) {
        }

        if (TWS_MT_SLA_ACK != TWSR_STATUS) {
            fai_pass_fail_logger(
                FAI_FAULT_ID_TWI_MT_SLAVE_ACK, 
                FAIL, 
                (U32_T) TWSR_STATUS
            );
            status = FAIL;
        } else {
            for (i = 0; i < len; ++i) {
                TWDR.byte = data[i];
                TWCR.byte = TWCR_TRANSMIT;

                /* Wait for data to be transmitted. */
                while (FALSE == TWCR.bits.TWINT) {
                }
            }

            if (TWS_MT_DATA_ACK != TWSR_STATUS) {
                fai_pass_fail_logger(
                    FAI_FAULT_ID_TWI_MT_DATA_ACK, 
                    FAIL, 
                    (U32_T) TWSR_STATUS
                );
                status = FAIL;
            } else {
                TWCR.byte = TWCR_STOP_CONDITION;
            }
        }
    }

    return status;
}


/*******************************************************************************
 * Transmit data into the given buffer from the given slave address via the TWI 
 * peripheral device.
 * \param[in] addr      The slave address to receive from.
 * \param[out] data     Array of bytes to be received in Master Receiver mode.
 * \param[in] len       Length in bytes to be received.
 * \return              The number of bytes receieved into `data`.
 ******************************************************************************/
SIZE_T twi_master_rx(U8_T addr, U8_T* data, SIZE_T len)
{
    SIZE_T i;

    i = 0;

    TWCR.byte = TWCR_START_CONDITION;

    /* Wait for start condition to be set. */
    while (FALSE == TWCR.bits.TWINT) {
    }

    if (TWCR_START_CONDITION != TWSR_STATUS) {
        fai_pass_fail_logger(FAI_FAULT_ID_TWI_START, FAIL, (U32_T) TWSR_STATUS);
    } else {
        TWDR.byte = TWDR_SLAVE_READ(addr);
        TWCR.byte = TWCR_TRANSMIT;

        /* Wait for address to be transmitted. */
        while (FALSE == TWCR.bits.TWINT) {
        }

        if (TWS_MR_SLA_ACK != TWSR_STATUS) {
            fai_pass_fail_logger(
                FAI_FAULT_ID_TWI_MR_SLAVE_ACK, 
                FAIL, 
                (U32_T) TWSR_STATUS
            );
        } else {
            /* Start receiving data */
            TWCR.byte = TWCR_ACKNOWLEDGE_DATA;

            for (i = 0; i < len; ++i) {
                /* Wait for data to be received. */
                while (FALSE == TWCR.bits.TWINT) {
                }

                data[i] = TWDR.byte;

                if (TWS_MR_DATA_ACK != TWSR_STATUS) {
                    fai_pass_fail_logger(
                        FAI_FAULT_ID_TWI_MR_DATA_ACK, 
                        FAIL, 
                        (U32_T) TWSR_STATUS
                    );
                    TWCR.byte = TWCR_ACKNOWLEDGE_DATA_END;
                    break;
                }

                if (i+1 < len) {
                    /* Continue receiving data */
                    TWCR.byte = TWCR_ACKNOWLEDGE_DATA;
                } else {
                    /* Stop receiving data */
                    TWCR.byte = TWCR_ACKNOWLEDGE_DATA_END;
                }
            }

            TWCR.byte = TWCR_STOP_CONDITION;
        }
    }

    return i;
}