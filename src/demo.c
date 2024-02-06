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
#include "twi.h"


static void demo_pk16(void);
static void demo_pwm(void);
static void demo_can_tx(void);
static void demo_adc_over_twi(void);


/*******************************************************************************
 * Initialize the demo application.
 ******************************************************************************/
void demo_init(void)
{
    /* 15-KHz PWM on PORTD6 */
    timer0_init(
        TIMER_MODE_FAST_PWM,
        TIMER0_PRESCALE_OVER_1024,
        TIMER0_OUTPUT_PIN_A_TOGGLE,
        TIMER0_INTERRUPTS_OFF
    );

    /* Enable the TWI peripheral device. */
    TWI_CFG_T cfg;
    cfg.prescale = TWI_PRESCALE_OVER_1;
    twi_init(cfg);

    /* Start CAN bus */
    can_init();

    /* Initiate conversion on sample and wait for it to finish before returning. */
    adc_init(ADC_MODE_BLOCKING);
    /* Select the appropriate ADC for sampling */
    adc_select(ADC_0);
}


/*******************************************************************************
 * Perform the demo transmit task.
 ******************************************************************************/ 
void task_demo_tx(void)
{
    demo_pwm();
    demo_can_tx();
    demo_pk16();
    /*
    demo_adc_over_twi();
    */
}


/*******************************************************************************
 * Perform the Package-16 task.
 ******************************************************************************/
static void demo_pk16(void)
{
    PK16_T pkg;
    U8_T buf[256];

    pk16_init(&pkg, buf, 256);
    pk16_add(&pkg, "/test.txt", (U8_T*) "Hello, world!", (U16_T) 13);
}


/*******************************************************************************
 * Perform the demo PWM task, incrementing and setting the duty cycle. 
 ******************************************************************************/ 
static void demo_pwm(void)
{
    static FLOAT_T duty;

    duty += 0.1f;
    if ( (1.0f < duty) || (0.0f > duty)) {
        duty = 0.0f;
    }
    timer0_pwm(duty);
}


/*******************************************************************************
 * Perform the demo CAN transmit task, enqueing a CAN message for transmission.
 ******************************************************************************/ 
static void demo_can_tx(void)
{
    U8_T buf[CAN_FIFO_DATA_LEN];
    SIZE_T len;
    FIFO_STATUS_T status;
    CAN_IDENT_T identifier;

    memcpy_by_U8(buf, (U8_T*) "Hello!", (SIZE_T) 6u);
    len = strnlen_by_U8((U8_T*) "Hello!", CAN_FIFO_DATA_LEN);

    identifier = (U16_T) 0b0110u;
    status = can_tx_q_add(identifier, buf, len);

    if (FIFO_OK == status) {
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, (U32_T) get_pc());
    } else {
        fai_pass_fail_logger(
            FAI_FAULT_ID_CAN_TX_BUFFER_OVERFLOW, 
            FAIL, 
            (U32_T) identifier
        );
    }
}

/*******************************************************************************
 * Perform the demo ADC/TWI task, sampling the ADC and transmitting its value 
 * over TWI.
 ******************************************************************************/ 
static void demo_adc_over_twi(void)
{
    U8_T buf[20];
    SIZE_T len;
    ADC_RESULT_T adc;

    /* Sample the ADC and stringify the result */
    adc = adc_sample();
    len = itoa(buf, (U32_T) adc);
    /* Add a newline */
    buf[len] = '\n';
    ++len;
    /* Transmit stringified value over TWI */
    twi_master_tx((U8_T) 0b10100000u, buf, len);
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