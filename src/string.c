#include "string.h"


/*******************************************************************************
 * Count the length of the given string, up to, but not including, a null
 * terminator ('\0').
 * \param[in] buf   The given string to be measured.
 * \param[in] max   The maximum number of bytes to be counted, acting as a range
 *                  bounds in case a null terminator is never found.
 * \return The length of the string in bytes.
 ******************************************************************************/
SIZE_T strnlen_by_U8(U8_T* buf, SIZE_T max)
{
    SIZE_T i;

    for (i = 0; (0 != buf[i]) && (i < max); ++i) {
    }

    return i;
}