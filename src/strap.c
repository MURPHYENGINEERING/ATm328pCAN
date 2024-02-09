#include "strap.h"
#include "types.h"


LANE_ID_T g_this_lane_id;
LANE_ID_T g_other_lane_id;

/*******************************************************************************
 * Initialize the device strapping by setting the correct pin modes.
 ******************************************************************************/
void strap_init(void)
{
    SIZE_T i;

    DDR_STRAPPING.bits.LANE = DDR_INPUT;
    PORT_STRAPPING.bits.LANE = PORT_PULLUP;

    /* Give the pullup time to settle */
    for (i = 0; i < 255; ++i) {
    }

    if (HIGH == PIN_STRAPPING.bits.LANE) {
        g_this_lane_id = LANE_ID_1;
        g_other_lane_id = LANE_ID_2;
    } else {
        g_this_lane_id = LANE_ID_2;
        g_other_lane_id = LANE_ID_1;
    }
}
