#include "memory.h"


/*******************************************************************************
 * Copy memory byte-by-byte from the given buffer into the given buffer, up to 
 * the given length.
 * \param[out] dst  The buffer into which memory will be copied.
 * \param[in] src   The buffer from which memory will be copied.
 * \param[in] len   The number of bytes to be copied from `src` to `dst`.
 * \return          The number of bytes copied.
 ******************************************************************************/
SIZE_T memcpy(void* dst, void* src, SIZE_T len)
{
    SIZE_T i;

    for (i = (SIZE_T) 0u; i < len; ++i) {
        *(U8_T*)dst = *(U8_T*)src;
        ++dst;
        ++src;
    }

    return i;
}


/*******************************************************************************
 * Set memory byte-by-byte to the given value, up to the given length.
 * \param[out] dst  The buffer into which the memory will be set.
 * \param[in] value The value to be set to all memory locations in the buffer.
 * \param[in] len   The number of bytes to be set in the destination buffer.
 ******************************************************************************/
void memset(void* dst, U8_T value, SIZE_T len)
{
    SIZE_T i;

    for (i = (SIZE_T) 0u; i < len; ++i) {
        *(U8_T*)dst = value;
        ++dst;
    }
}


/*******************************************************************************
 * Compare the left-hand buffer to the right-hand buffer, up to the given length.
 * \param[in] lhs   The left-hand side buffer to be compared.
 * \param[in] rhs   The right-hand side buffer to be compared.
 * \param[in] len   The number of bytes to be compared between both buffers.
 * \retval          0 if the buffers are equal up to `len` bytes.
 * \retval          <0 if `lhs` < `rhs` at the first unequal byte.
 * \retval          >0 if `lhs` > `rhs` at the first unequal byte.
 ******************************************************************************/
S8_T memcmp(void* lhs, void* rhs, SIZE_T len)
{
    SIZE_T i;
    S8_T result;

    i = (SIZE_T) 0u;
    result = *(S8_T*)lhs - *(S8_T*)rhs;

    while ( ((S32_T) 0u == result) && (i < len) ) {
        result = *(S8_T*)lhs - *(S8_T*)rhs;
        ++lhs;
        ++rhs;
        ++i;
    }

    return result;
}


/*******************************************************************************
 * Get the current Program Counter address at the call site.
 * \return The Program Counter value to which this function returns.
 ******************************************************************************/
U16_T get_pc(void) 
{
    return (U16_T)__builtin_return_address(0);
}