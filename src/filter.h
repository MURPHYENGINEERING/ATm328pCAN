#ifndef __FILTER_H__
#define __FILTER_H__

#include "types.h"

FLOAT_T filter_iir_1o(FLOAT_T raw, FLOAT_T filtered, FLOAT_T weight);

#endif