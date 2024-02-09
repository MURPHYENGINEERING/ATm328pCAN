#ifndef __STRAP_H__
#define __STRAP_H__

#include "types.h"


/** Represents the strapping (hard-wired configuration) GPIO registers. */
typedef union {
    struct {
        VBOOL_T Unused0 : 1;
        VBOOL_T Unused1 : 1;
        VBOOL_T LANE    : 1;
        VBOOL_T Unused3 : 1;
        VBOOL_T Unused4 : 1;
        VBOOL_T Unused5 : 1;
        VBOOL_T Unused6 : 1;
        VBOOL_T Unused7 : 1;
    } bits;
    VU8_T byte;
} STRAPPING_T;

/** Data Direction Register for the strapping GPIOs */
extern volatile STRAPPING_T DDR_STRAPPING;
/** Output Port Register for the strapping GPIOs */
extern volatile STRAPPING_T PORT_STRAPPING;
/** Input Pin Register for the strapping GPIOs */
extern volatile STRAPPING_T PIN_STRAPPING;


/** Represents the strapped lane ID for this device. */
typedef enum {
    LANE_ID_1 = 0u,
    LANE_ID_2 = 1u
} LANE_ID_T;


/** This lane's ID as determined by strapping on startup. */
extern LANE_ID_T g_this_lane_id;
/** The opposing lane's ID as determined by strapping on startup. */
extern LANE_ID_T g_other_lane_id;


void strap_init(void);


#endif