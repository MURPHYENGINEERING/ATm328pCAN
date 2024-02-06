#include "pk16.h"
#include "types.h"
#include "memory.h"



/*******************************************************************************
 * Initialize the given package with the given underlying buffer.
 * \param[out] pkg  The package to initialize to the given buffer.
 * \param[in] buf   The underlying buffer the package (with header) will be 
 *                  written into.
 * \param[in] len   The length in bytes of the underlying buffer.
 ******************************************************************************/
void pk16_init(PK16_T* pkg, U8_T* buf, SIZE_T len)
{
    memset(pkg, (U8_T) 0u, sizeof(PK16_T));

    pkg->header.magic = 0xDEADBEEF;
    pkg->buf = buf;
    pkg->size = len;

    memset_by_U8(buf, (U8_T) 0u, len);
}



/*******************************************************************************
 * Add an entry into the given Package-16 package. 
 * \param[out] pkg  The package into which to add an entry.
 * \param[in] path  The path at which the given entry can be found.
 * \param[in] data  The contents of the entry.
 * \param[in] len   The length in bytes of the `data` array.
 * \return `PK16_OK` if the entry was added or `PK16_FULL` if the package is full.
 ******************************************************************************/
void pk16_add(PK16_T* pkg, CSTR_T path, U8_T* data, SIZE_T len)
{
}




/*******************************************************************************
 * Flush the given package into its underlying buffer, writing all pending changes.
 ******************************************************************************/
void pk16_flush(PK16_T* pkg)
{
    memcpy_by_U8(pkg->buf, &pkg->header, sizeof(PK16_HEADER_T));
}