#include "types.h"
#include "filter.h"
#include "fai.h"
#include "memory.h"


/*******************************************************************************
 * Infinite Impulse Response (IIR) filter. Computes the current filtered value
 * by taking a raw value, a previous filtered value, and a weight (percentage),
 * and applying the following equation:\n
 *      Y[t] = (W)(X[t]) + (1 - W)(Y[t-1]), where\n
 *      Y[t]    = Current filtered value,\n
 *      W       = Weight,\n
 *      X[t]    = Raw value,\n
 *      Y[t-1]  = Previous filtered value.
 * \param[in] raw       X[t], the unfiltered value.
 * \param[in] filtered  Y[t-1], the previous filtered value.
 * \param[in] weight    W, the percentage of the raw value to incorporate vs. the
 *                      previously filtered value.
 * \return              Y[t], the filtered value.
 ******************************************************************************/
FLOAT_T filter_iir_1o(FLOAT_T raw, FLOAT_T filtered, FLOAT_T weight)
{
    FLOAT_T y_t;

    if ( (0.0f > weight) || (1.0f < weight) ) {
        fai_pass_fail_logger(FAI_FAULT_ID_SW_ERROR, FAIL, get_pc());
        y_t = filtered;
    } else {
        if ( ZERO_EPSILON >= raw ) {
            raw = 0.0f;
        }

        if ( ZERO_EPSILON >= filtered ) {
            y_t = raw;
        } else {
            y_t = (weight * raw) + ((1.0f - weight) * filtered);
        }
    }
    
    return y_t;
}