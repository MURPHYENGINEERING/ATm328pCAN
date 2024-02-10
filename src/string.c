#include "string.h"
#include "types.h"

#define MAX_PRECISION 10u
static const DOUBLE_T rounders[MAX_PRECISION + 1] =
{
    0.5,
    0.05,
    0.005,
    0.0005,
    0.00005,
    0.000005,
    0.0000005,
    0.00000005,
    0.000000005,
    0.0000000005,
    0.00000000005
};


SIZE_T dtoa(CSTR_T buf, DOUBLE_T f, U16_T precision)
{
    CSTR_T ptr = buf;
    CSTR_T p = ptr;
    CSTR_T p1;
    CHAR_T c;
    S32_T intPart;

    // check precision bounds
    if (MAX_PRECISION < precision) {
        precision = MAX_PRECISION;
    }

    // sign stuff
    if (f < 0) {
        f = -f;
        *ptr++ = '-';
    }

    if (0 > precision) { // negative precision == automatic precision guess
        if (1.0 > f) { precision = 6; }
        else if (10.0 > f) { precision = 5; }
        else if (100.0 > f) { precision = 4; }
        else if (1000.0 > f) { precision = 3; }
        else if (10000.0 > f) { precision = 2; }
        else if (100000.0 > f) { precision = 1; }
        else { precision = 0; }
    }

    // round value according the precision
    if (precision) {
        f += rounders[precision];
    }

    // integer part...
    intPart = f;
    f -= intPart;

    if (0 == intPart) {
        *ptr++ = '0';
    } else {
        // save start pointer
        p = ptr;

        // convert (reverse order)
        while (intPart) {
            *p++ = '0' + intPart % 10;
            intPart /= 10;
        }

        // save end pos
        p1 = p;

        // reverse result
        while (p > ptr) {
            c = *--p;
            *p = *ptr;
            *ptr++ = c;
        }

        // restore end pos
        ptr = p1;
    }

    // decimal part
    if (precision) {
        // place decimal point
        *ptr++ = '.';

        // convert
        while (precision--) {
            f *= 10.0;
            c = f;
            *ptr++ = '0' + c;
            f -= c;
        }
    }

    // terminating zero
    *ptr = '\0';

    return (SIZE_T)( ptr - buf );
}


/*******************************************************************************
 * Copy the `s_src` string into the `s_dst` string buffer, up to `max` characters,
 * ensuring the resulting string is null terminated.
 * \param[in] s_dst   The given string to be measured.
 * \param[in] s_src   The maximum number of bytes to be counted, acting as a range
 *                  bounds in case a null terminator is never found.
 * \param[in] max   The maximum number of characters to be copied, including the
 *                  null terminator.
 * \return          The length of the copied string in characters.
 ******************************************************************************/
SIZE_T strncpy(CSTR_T s_dst, CSTR_T s_src, SIZE_T max)
{
    SIZE_T i;

    for (i = (SIZE_T) 0u; ((CHAR_T) '\0' != s_src[i]) && (i < max-1); ++i) {
        s_dst[i] = s_src[i];
    }

    s_dst[i] = (CHAR_T) '\0';

    return i;
}


/*******************************************************************************
 * Count the length of the given string `s` up to, but not including, the null
 * terminator ('\0').
 * \param[in] s     The given string to be measured.
 * \param[in] max   The maximum number of bytes to be counted, acting as a range
 *                  bounds in case a null terminator is never found.
 * \return          The length of the string in bytes.
 ******************************************************************************/
SIZE_T strnlen(CSTR_T s, SIZE_T max)
{
    SIZE_T i;

    for (i = (SIZE_T) 0u; ('\0' != s[i]) && (i < max); ++i) {
    }

    return i;
}


CHAR_T strncmp(CSTR_T s_lhs, CSTR_T s_rhs, SIZE_T max)
{
    CHAR_T cmp;

    while ( (0 < max) && (0 != *s_lhs) && (*s_lhs == *s_rhs) ) {
        ++s_lhs;
        ++s_rhs;
        --max;
    }
    if (0 == max) {
        cmp = 0;
    } else {
        cmp = *((U8_T*)s_lhs) - *((U8_T*)s_rhs);
    }

    return cmp;
}


/*******************************************************************************
 * Reverse the given string in place, leaving the trailing zero in place.
 * \param[out] s The string to be reversed. 
 ******************************************************************************/
void strreverse(CSTR_T s)
{
    SIZE_T i;
    SIZE_T j;
    S8_T c;

    j = strnlen(s, SIZE_T_MAX) - 1;

    for (i = (SIZE_T) 0u; i < j; ++i) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
        --j;
    }
}


/*******************************************************************************
 * Generate a string representation of the given integer value in the given buffer.
 * \param[out] s    The buffer into which the string representation will be written.
 * \param[in]  n    The integer value to be converted.
 * \return The length of the string created, not including the trailing zero.
 ******************************************************************************/
SIZE_T itoa(CSTR_T s, U32_T n)
{
    SIZE_T i;
 
    i = (SIZE_T) 0u;

    do { 
        s[i] = n % 10u + '0';
        ++i;
    } while ((n /= 10u) > 0u);

    s[i] = (CHAR_T) '\0';

    strreverse(s);

    return i;
}