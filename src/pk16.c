#include "pk16.h"
#include "types.h"
#include "memory.h"
#include "string.h"
#include "crc.h"


/** Magic number identifying an array of bytes as a PK16 package. */
#define PK16_MAGIC (U16_T) 0xEFBEu
/** Current version of this PK16 library. */
#define PK16_VERSION (U8_T) 1u


/*******************************************************************************
 * Initialize the given package with the given underlying buffer.
 * \param[out] p_pkg    The package to initialize to the given buffer.
 * \param[in] p_buf     The underlying buffer the package (with header) will be 
 *                      written into.
 * \param[in] size      The length in bytes of the underlying buffer.
 ******************************************************************************/
void pk16_init(PK16_T* p_pkg, U8_T* p_buf, SIZE_T size)
{
    PK16_HEADER_T* p_header;

    /* Clear the package structure */
    memset(p_pkg, (U8_T) 0u, sizeof(PK16_T));

    /* Clear the package buffer */
    p_pkg->buf = p_buf;
    memset(p_pkg->buf, (U8_T) 1u, size);

    /* Set the package buffer size */
    p_pkg->size = size;

    /* Clear the package header */
    p_header = (PK16_HEADER_T*) p_pkg->buf;
    p_header->magic = PK16_MAGIC;
    p_header->version = PK16_VERSION;
    p_header->data_len = (U16_T) 0u;
    p_header->n = (U16_T) 0u;
}



/*******************************************************************************
 * Add an entry into the given PK16 package. 
 * \param[out] p_pkg    The package into which to add an entry.
 * \param[in] s_path      The s_path at which the given entry can be found.
 * \param[in] p_data    The contents of the entry.
 * \param[in] len       The length in bytes of the `p_data` array.
 * \retval              `PK16_OK` if the entry was added.
 * \retval              `PK16_FULL` if the package is full.
 ******************************************************************************/
PK16_RESULT_T pk16_add(PK16_T* p_pkg, CSTR_T s_path, U8_T* p_data, SIZE_T len)
{
    PK16_RESULT_T result;
    PK16_HEADER_T* p_header;
    PK16_TABLE_T* p_table; 
    SIZE_T old_table_head;
    SIZE_T new_table_head;
    SIZE_T old_table_tail;
    SIZE_T new_table_tail;
    SIZE_T table_size;
    SIZE_T i;

    result = PK16_FULL;

    p_header = (PK16_HEADER_T*) p_pkg->buf;

    if (PK16_MAGIC != p_header->magic) {
        result = PK16_NOT_A_PACKAGE;
    } else if (PK16_VERSION != p_header->version) {
        result = PK16_WRONG_VERSION;
    }
    if (PK16_FULL == result) {
        /* Check if there's an existing entry at this path */
        p_table = pk16_find_table_by_path(p_pkg, s_path);

        if (NULL != p_table) {
            result = PK16_EXISTS;
        } 
    }
    if (PK16_FULL == result) {
        /* Still OK, add the entry to the package */

        table_size = sizeof(PK16_TABLE_T) * p_header->n;

        /* If the size of the header + the existing data + this new data
            * + the existing table entries + this new table entry doesn't exceed the
            * underlying buffer size... */
        if ((SIZE_T)(sizeof(PK16_HEADER_T) 
            + p_header->data_len 
            + len 
            + table_size + sizeof(PK16_TABLE_T)
            ) < p_pkg->size)
        {
            /* Locate where the table is and where it should be after the new data
                * are added */
            /* The table is currently located at the end of the header and data. */
            old_table_head = (SIZE_T)( sizeof(PK16_HEADER_T) + p_header->data_len );
            /* Its new position should be increased by the length of the new data. */
            new_table_head = (SIZE_T)( old_table_head + len );
            /* Find the tails of the table; this is where we'll copy from and to. */
            old_table_tail = 
                (SIZE_T)( old_table_head + table_size - 1 );
            new_table_tail = 
                (SIZE_T)( new_table_head + table_size - 1 );

            /* Move the entire table to make room for the new data. */
            /* We have to copy from the tail so we don't overwrite before copy. */
            for (i = 0; table_size > i; ++i) {
                p_pkg->buf[new_table_tail - i] = p_pkg->buf[old_table_tail - i];
            }

            /* Copy in the new data. The old table head points to the end of the 
                * existing data. */
            memcpy(&p_pkg->buf[old_table_head], p_data, len);
            /* Add the new data to the header */
            p_header->data_len += len;

            /* Write the new table entry into the end of the table */
            p_table = pk16_find_table_by_index(p_pkg, p_header->n);
            strncpy(p_table->s_path, s_path, PK16_MAX_PATH_LEN);
            /* The new entry's data goes where the table was, at the end of the
                * existing data. */
            p_table->head = old_table_head;
            p_table->len = len;
            p_table->checksum = crc_compute_checksum32(p_data, len, (U32_T) 0u);

            /* Add the new table entry to the header */
            ++p_header->n;

            result = PK16_OK;
        }
    }

    return result;
}


/*******************************************************************************
 * Read the specified entry data from the package.
 * \param[in] pkg   The package to read from.
 * \param[in] s_path  The s_path of the entry to read.
 * \param[out] dst  The buffer into which to read the data.
 * \param[in] max   The maximum number of bytes to read.
 * \return          The number of bytes read.
 ******************************************************************************/
SIZE_T pk16_read(PK16_T* p_pkg, CSTR_T s_path, U8_T* p_dst, SIZE_T max)
{
    SIZE_T bytes_read;
    PK16_TABLE_T* p_table;

    bytes_read = (SIZE_T) 0u;

    p_table = pk16_find_table_by_path(p_pkg, s_path);

    if ((PK16_TABLE_T*) NULL != p_table) {
        if (max > p_table->len) {
            max = p_table->len;
        }
        memcpy(p_dst, &p_pkg->buf[p_table->head], max);
        bytes_read = max;
    }

    return bytes_read;
}


/*******************************************************************************
 * Get a pointer to a table entry given its path in the table.
 * \param[out] p_pkg    The package in which to locate the table entry.
 * \param[in] s_path    The s_path of the table entry to be located.
 * \retval              A pointer to the located table entry.
 * \retval              `NULL` if the given path is not found.
 ******************************************************************************/
PK16_TABLE_T* pk16_find_table_by_path(PK16_T* p_pkg, CSTR_T s_path)
{
    PK16_HEADER_T* p_header;
    PK16_TABLE_T* p_table;
    SIZE_T i;
    
    p_table = (PK16_TABLE_T*) NULL;

    p_header = (PK16_HEADER_T*) p_pkg->buf;

    for (i = 0; i < p_header->n; ++i) {
        p_table = pk16_find_table_by_index(p_pkg, i);
        if (0 == strncmp(s_path, p_table->s_path, PK16_MAX_PATH_LEN)) {
            break;
        }
        p_table = (PK16_TABLE_T*) NULL; 
    }

    return p_table;
}


/*******************************************************************************
 * Get a pointer to a table entry given its index in the table.
 * \param[out] p_pkg    The package in which to locate the table entry.
 * \param[in] index     The index of the table entry to be located.
 * \retval              A pointer to the located table entry.
 * \retval              `NULL` if the given index is out of range.
 ******************************************************************************/
PK16_TABLE_T* pk16_find_table_by_index(PK16_T* p_pkg, SIZE_T index)
{
    SIZE_T offset;
    PK16_TABLE_T* p_table;
    PK16_HEADER_T* p_header;

    p_header = (PK16_HEADER_T*) p_pkg->buf;

    offset = (SIZE_T)( sizeof(PK16_HEADER_T) 
                        + p_header->data_len 
                        + sizeof(PK16_TABLE_T) * index );

    if ((SIZE_T)( p_pkg->size - sizeof(PK16_TABLE_T) ) > offset) {
        p_table = (PK16_TABLE_T*) &p_pkg->buf[offset];
    } else {
        p_table = (PK16_TABLE_T*) NULL;
    }

    return p_table;
}