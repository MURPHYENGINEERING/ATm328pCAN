#include "atm328p_twi.h"
#include "twi.h"


/*******************************************************************************
 * Initialize the TWI peripheral device with the given configuration.
 * \param[in] cfg   The configuration to which the TWI peripheral device will be 
 *                  initialized.
 ******************************************************************************/
void twi_init_hardware(TWI_CFG_T cfg)
{
}


/*******************************************************************************
 * Transmit the given data to the given slave address on the TWI peripheral 
 * device.
 * \param[in] addr  The slave address to transmit to.
 * \param[in] data  Array of bytes to be transmitted in Master Transmitter mode.
 * \param[in] len   Length in bytes of the `data` array.
 ******************************************************************************/
void twi_master_tx(U8_T addr, U8_T* data, SIZE_T len)
{
    SIZE_T i;

    TWCR.byte = TWCR_START_CONDITION;

    /* Wait for start condition to be set. */
    while (FALSE == TWCR.bits.TWINT) {
    }

    if (TWCR_START_CONDTIION != TWSR_STATUS) {
        fai_pass_fail_logger(FAI_FAULT_ID_TWI_START, FAIL, (U32_T) TWSR_STATUS);
    } else {
        /* Load the address with the Slave Write bit set and tansmit */
        TWDR.byte = TWDR_SLAVE_WRITE(addr);
        TWCR.byte = TWCR_TRANSMIT;

        /* Wait for address to be transmitted. */
        while (FALSE == TWCR.bits.TWINT) {
        }

        if (TWS_MT_SLAVE_ACK != TWSR_STATUS) {
            fai_pass_fail_logger(FAI_FAULT_ID_TWI_MT_SLAVE_ACK, FAIL, (U32_T) TWSR_STATUS);
        } else {
            for (i = 0; i < len; ++i) {
                TWDR.byte = data[i];
                TWCR.byte = TWCR_TRANSMIT;

                /* Wait for data to be transmitted. */
                while (FALSE == TWCR.bits.TWINT) {
                }
            }

            if (TWS_MT_DATA_ACK != TWSR_STATUS) {
                fai_pass_fail_logger(FAI_FAULT_ID_TWI_MT_DATA_ACK, FAIL, (U32_T) TWSR_STATUS);
            } else {
                TWCR.byte = TWCR_STOP_CONDITION;
            }
        }
    }
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
    SIZE_T received_len;

    i = 0;

    TWCR.byte = TWCR_START_CONDITION;

    /* Wait for start condition to be set. */
    while (FALSE == TWCR.bits.TWINT) {
    }

    if (TWCR_START_CONDTIION != TWSR_STATUS) {
        fai_pass_fail_logger(FAI_FAULT_ID_TWI_START, FAIL, (U32_T) TWSR_STATUS);
    } else {
        TWDR.byte = TWDR_SLAVE_READ(addr);
        TWCR.byte = TWCR_TRANSMIT;

        /* Wait for address to be transmitted. */
        while (FALSE == TWCR.bits.TWINT) {
        }

        if (TWS_MR_SLAVE_ACK != TWSR_STATUS) {
            fai_pass_fail_logger(FAI_FAULT_ID_TWI_MR_SLAVE_ACK, FAIL, (U32_T) TWSR_STATUS);
        } else {
            /* Start receiving data */
            TWCR.byte = TWCR_ACKNOWLEDGE_DATA;

            for (i = 0; i < len; ++i) {
                /* Wait for data to be received. */
                while (FALSE == TWCR.bits.TWINT) {
                }

                if (TWS_MR_DATA_ACK != TWSR_STATUS) {
                    fai_pass_fail_logger(FAI_FAULT_ID_TWI_MR_SLAVE_ACK, FAIL, (U32_T) TWSR_STATUS);
                    break;
                }

                data[i] = TWDR.byte;

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