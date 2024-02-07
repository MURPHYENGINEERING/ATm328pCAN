#include "string.h"
#include "types.h"


/*******************************************************************************
 * Copy the `src` string into the `dst` string buffer, up to `max` characters,
 * ensuring the resulting string is null terminated.
 * \param[in] dst   The given string to be measured.
 * \param[in] src   The maximum number of bytes to be counted, acting as a range
 *                  bounds in case a null terminator is never found.
 * \param[in] max   The maximum number of characters to be copied, including the
 *                  null terminator.
 * \return          The length of the copied string in characters.
 ******************************************************************************/
SIZE_T strncpy(CSTR_T dst, CSTR_T src, SIZE_T max)
{
    SIZE_T i;

    for (i = (SIZE_T) 0u; ((CHAR_T) '\0' != src[i]) && (i < max-1); ++i) {
        dst[i] = src[i];
    }

    dst[i] = (CHAR_T) '\0';

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


CHAR_T strncmp(CSTR_T lhs, CSTR_T rhs, SIZE_T max)
{
    CHAR_T cmp;

    while ( (0 < max) && (0 != *lhs) && (*lhs == *rhs) ) {
        ++lhs;
        ++rhs;
        --max;
    }
    if (0 == max) {
        cmp = 0;
    } else {
        cmp = *((U8_T*)lhs) - *((U8_T*)rhs);
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