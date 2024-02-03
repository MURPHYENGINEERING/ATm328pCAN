#include "types.h"
#include "demo.h"
#include "fifo.h"
#include "can.h"
#include "memory.h"
#include "string.h"
#include "dsc.h"
#include "fai.h"
#include "usart.h"
#include "adc.h"
#include "timer.h"


/*******************************************************************************
 * Initialize the demo application.
 ******************************************************************************/
void demo_init(void)
{
    /* 15-KHz PWM on PORTD6 */
    /*
    timer0_init(
        TIMER_MODE_FAST_PWM,
        TIMER0_PRESCALE_OVER_1024,
        TIMER0_OUTPUT_PIN_A_TOGGLE,
        TIMER0_INTERRUPTS_OFF
    );
    */
}


/*******************************************************************************
 * Perform the demo transmit task, writing messages to the CAN software FIFO.
 ******************************************************************************/ 
void task_demo_tx(void)
{
    U8_T buf[CAN_FIFO_DATA_LEN];
    SIZE_T len;
    FIFO_STATUS_T status;
    CAN_IDENT_T identifier;
    /*
    ADC_RESULT_T adc;
    */
    /*
    static FLOAT_T duty;

    duty += 0.1f;
    if (duty > 1.0f) {
        duty = 0.0f;
    }
    timer0_pwm(duty);
    */

    identifier = (U16_T) 0b0110u;

    memcpy_by_U8(buf, (U8_T*) "Hello!", (SIZE_T) 6u);
    len = strnlen_by_U8((U8_T*) "Hello!", CAN_FIFO_DATA_LEN);

    status = can_tx_q_add(identifier, buf, len);

    if (FIFO_OK == status) {
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, (U32_T) 0u);
        /*
        adc = adc_sample();
        len = itoa(buf, (U32_T) adc);
        buf[len] = '\n';
        usart_tx(buf, len+1);
        */
    } else {
        fai_pass_fail_logger(
            FAI_FAULT_ID_CAN_TX_BUFFER_OVERFLOW, 
            FAIL, 
            (U32_T) identifier
        );
    }
}


/*******************************************************************************
 * Perform the demo receive task, accepting messages from the CAN software FIFO
 * and writing them to the USART.
 ******************************************************************************/ 
void task_demo_rx(void)
{
    SIZE_T n_pending_msgs;
    SIZE_T i;
    FIFO_STATUS_T status;
    U8_T buf[CAN_FIFO_DATA_LEN];
    SIZE_T len;
    CAN_IDENT_T identifier;

    n_pending_msgs = can_rx_q_len();

    for (i = 0; i < n_pending_msgs; ++i) {
        status = can_rx_q_remove(&identifier, buf, &len);

        if (FIFO_OK == status) {
            usart_tx(buf, len);
            
        } else {
            fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
        }
    }
}