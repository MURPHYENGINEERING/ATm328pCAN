#include "pk16.h"
#include "types.h"
#include "memory.h"
#include "string.h"
#include "crc.h"



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

    pkg->header.magic = (U16_T) 0xBEEFu;
    pkg->header.version = (U8_T) 1u;

    pkg->buf = buf;
    pkg->size = len;

    memset(buf, (U8_T) 0u, len);
}



/*******************************************************************************
 * Add an entry into the given Package-16 package. 
 * \param[out] pkg  The package into which to add an entry.
 * \param[in] path  The path at which the given entry can be found.
 * \param[in] data  The contents of the entry.
 * \param[in] len   The length in bytes of the `data` array.
 * \return `PK16_OK` if the entry was added or `PK16_FULL` if the package is full.
 ******************************************************************************/
PK16_RESULT_T pk16_add(PK16_T* pkg, CSTR_T path, U8_T* data, SIZE_T len)
{
    PK16_RESULT_T result;
    PK16_TABLE_T* table; 

    result = PK16_FULL;

    if ((sizeof(PK16_HEADER_T) + pkg->header.data_len + len + sizeof(PK16_TABLE_T)) 
        < pkg->size) {

        /* Copy the given data buffer into the underlying buffer... */
        memcpy(&pkg->buf[pkg->header.data_len], data, len);

        /* Go to the last entry in the table */
        table = (PK16_TABLE_T*)( 
              pkg->buf 
            + sizeof(PK16_HEADER_T) 
            + pkg->header.data_len + len
            + sizeof(PK16_TABLE_T) * pkg->header.n 
        );

        /* Update the table entry re: data*/
        table->head = (U16_T)( pkg->header.data_len - len );
        table->len = len;
        table->path_len = strnlen(path, PK16_MAX_PATH_LEN);
        memcpy(&pkg->buf[table->head], path, table->path_len);
        table->crc = crc_compute_crc32(data, len, (U32_T) 0u, TRUE);

        /* Step forward in the underlying buffer by len bytes */
        pkg->header.data_len += len;
        /* Increment the number of entries in the table */
        ++pkg->header.n;
    
        result = PK16_OK;
    }

    return result;
}