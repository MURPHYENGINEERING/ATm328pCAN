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
    p_pkg->p_buf = p_buf;
    memset(p_pkg->p_buf, (U8_T) 0xFFu, size);

    /* Set the package buffer size */
    p_pkg->size = size;

    /* Clear the package header */
    p_header = (PK16_HEADER_T*) p_pkg->p_buf;
    p_header->magic = PK16_MAGIC;
    p_header->version = PK16_VERSION;
    p_header->data_len = (U16_T) 0u;
    p_header->n = (U16_T) 0u;
}



/*******************************************************************************
 * Add an entry into the given PK16 package. 
 * \param[out] p_pkg    The package into which to add an entry.
 * \param[in] s_path    The path at which the given entry will be found.
 * \param[in] p_data    The contents of the entry.
 * \param[in] len       The length in bytes of the `p_data` array.
 * \retval              `PK16_OK` if the entry was added.
 * \retval              `PK16_FULL` if the package is full.
 * \retval              `PK16_EXISTS` if an entry already exists at the given path.
 * \retval              `PK16_NOT_A_PACKAGE` if the given package is not valid. 
 * \retval              `PK16_WRONG_VERSION` if the given package was generated
 *                      by a different version of the PK16 library.
 ******************************************************************************/
PK16_RESULT_T pk16_add(PK16_T* p_pkg, CSTR_T s_path, U8_T* p_data, SIZE_T len)
{
    PK16_RESULT_T result;
    PK16_HEADER_T* p_header;
    PK16_TABLE_T* p_table; 
    U8_T* p_old_table_head;
    U8_T* p_new_table_head;
    U8_T* p_old_table_tail;
    U8_T* p_new_table_tail;
    SIZE_T tables_size;
    SIZE_T i;

    result = PK16_FULL;

    p_header = (PK16_HEADER_T*) p_pkg->p_buf;

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

        tables_size = sizeof(PK16_TABLE_T) * p_header->n;

        /* If the size of the header + the existing data + this new data
            * + the existing table entries + this new table entry doesn't exceed the
            * underlying buffer size... */
        if ((SIZE_T)(sizeof(PK16_HEADER_T) 
            + p_header->data_len 
            + len 
            + tables_size + sizeof(PK16_TABLE_T)
            ) < p_pkg->size)
        {
            /* Locate where the table is and where it should be after the new data
                * are added */
            /* The table is currently located at the end of the header and data. */
            p_old_table_head = (U8_T*)( p_pkg->p_buf + sizeof(PK16_HEADER_T) + p_header->data_len );
            /* Its new position should be increased by the length of the new data. */
            p_new_table_head = (U8_T*)( p_old_table_head + len );
            /* Find the tails of the table; this is where we'll copy from and to. */
            p_old_table_tail = (U8_T*)( p_old_table_head + tables_size - 1 );
            p_new_table_tail = (U8_T*)( p_new_table_head + tables_size - 1 );

            /* Move the entire table to make room for the new data. */
            /* We have to copy from the tail so we don't overwrite before copy. */
            for (i = 0; tables_size > i; ++i) {
                *(U8_T*)(p_new_table_tail - i) = *(U8_T*)(p_old_table_tail - i);
            }

            /* Copy in the new data. The old table head points to the end of the 
                * existing data. */
            memcpy(p_old_table_head, p_data, len);
            /* Add the new data to the header */
            p_header->data_len += len;

            /* Write the new table entry into the end of the table */
            p_table = pk16_find_table_by_index(p_pkg, p_header->n);
            p_table->s_path = s_path;
            /* The new entry's data goes where the table was, at the end of the
                * existing data. */
            p_table->head = (SIZE_T)( p_old_table_head - p_pkg->p_buf );
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
 * Remove the entry at the specified path from the given package.
 * \param[in] pkg       The package to remove an entry from.
 * \param[in] s_path    The path of the entry to read.
 * \retval              `PK16_OK` if the entry was removed.
 * \retval              `PK16_EMPTY` if the entry was not found in the package.
 * \retval              `PK16_NOT_A_PACKAGE` if the given package is not valid.
 * \retval              `PK16_WRONG_VERSION` if the given package was generated
 *                      by a different version of the PK16 library.
 ******************************************************************************/
PK16_RESULT_T pk16_remove(PK16_T* p_pkg, CSTR_T s_path)
{
    PK16_RESULT_T result;
    PK16_HEADER_T* p_header;
    PK16_TABLE_T* p_table;
    U8_T* p_cursor;
    U8_T* p_trailing;
    SIZE_T trailing_len;
    SIZE_T i;
    SIZE_T removed_len;
    BOOL_T relocate_heads;

    result = PK16_EMPTY;

    p_header = (PK16_HEADER_T*) p_pkg->p_buf;

    if (PK16_MAGIC != p_header->magic) {
        result = PK16_NOT_A_PACKAGE;
    } else if (PK16_VERSION != p_header->version) {
        result = PK16_WRONG_VERSION;
    }
    if (PK16_EMPTY == result) {
        /* Check if there's an existing entry at this path */
        p_table = pk16_find_table_by_path(p_pkg, s_path);

        if (NULL != p_table) {
            result = PK16_OK;
        }
    }
    if (PK16_OK == result) {
        /* Move all the trailing data up to fill the removed space. */ 
        removed_len = p_table->len;

        p_cursor = &p_pkg->p_buf[p_table->head];
        p_trailing = (U8_T*)( p_cursor + removed_len );
        trailing_len = sizeof(PK16_HEADER_T) 
                     + p_header->data_len 
                     - p_table->head 
                     - removed_len;

        for (i = (SIZE_T) 0u; i < trailing_len; ++i) {
            *p_cursor = *p_trailing;
            ++p_cursor;
            ++p_trailing;
        }

        /* Point to the new location for the table */
        p_cursor = (U8_T*)( 
            p_pkg->p_buf + sizeof(PK16_HEADER_T) + p_header->data_len - removed_len
        );

        relocate_heads = FALSE;
        /* Move the table elements up to the end of the data, excluding the
         * removed one. */
        for (i = (SIZE_T) 0u; i < p_header->n; ++i) {
            p_table = pk16_find_table_by_index(p_pkg, i);
            if ( 0 != strncmp(p_table->s_path, s_path, PK16_MAX_PATH_LEN) ) {
                /* Make sure we update the head BEFORE moving the table...lol */
                if (TRUE == relocate_heads) {
                    p_table->head -= removed_len;
                }
                memcpy(p_cursor, (U8_T*) p_table, sizeof(PK16_TABLE_T));
                p_cursor += sizeof(PK16_TABLE_T);
            } else {
                relocate_heads = TRUE;
            }
        }

        p_header->data_len -= removed_len;
        --p_header->n;
    }

    return result;
}


/*******************************************************************************
 * Read the specified entry data from the package.
 * \param[in] pkg   The package to read from.
 * \param[in] s_path  The path of the entry to read.
 * \param[out] dst  The buffer into which to read the data.
 * \param[in] max   The maximum number of bytes to read.
 * \return          The number of bytes read.
 ******************************************************************************/
SIZE_T pk16_read(PK16_T* p_pkg, CSTR_T s_path, U8_T* p_dst, SIZE_T max)
{
    SIZE_T bytes_read;
    PK16_TABLE_T* p_table;
    U32_T checksum;

    bytes_read = (SIZE_T) 0u;

    p_table = pk16_find_table_by_path(p_pkg, s_path);

    if ((PK16_TABLE_T*) NULL != p_table) {
        checksum = crc_compute_checksum32(
                        &p_pkg->p_buf[p_table->head], 
                        p_table->len, 
                        (U32_T) 0u
                    );
        if (checksum == p_table->checksum) {
            if (max > p_table->len) {
                max = p_table->len;
            }
            memcpy(p_dst, &p_pkg->p_buf[p_table->head], max);
            bytes_read = max;
        }
    }

    return bytes_read;
}


/*******************************************************************************
 * Serialize the given package into the given destination buffer, replacing path
 * pointers with null-terminated strings.
 * \param[in]  p_pkg        The package to be serialized.
 * \param[out] p_dst        The destination buffer for the serialized data.
 * \param[out] final_len    The length in bytes of the serialized data.
 * \param[in]  max          The maximum number of bytes that can be written into 
 *                          the destination buffer.
 * \retval  `PK16_OK` if the package was serialized into the buffer.
 * \retval  `PK16_FULL` if the buffer is not long enough to store the
 *          serialized package.
 ******************************************************************************/
PK16_RESULT_T pk16_serialize(PK16_T* p_pkg, U8_T* p_dst, SIZE_T* final_len, SIZE_T max)
{
    PK16_RESULT_T result;
    U8_T* p_start;
    U8_T* p_end;
    PK16_HEADER_T* p_header;
    PK16_TABLE_T* p_table;
    SIZE_T basic_len;
    SIZE_T i;
    SIZE_T j;
    SIZE_T path_len;
    BOOL_T wrote_whole_path;

    result = PK16_FULL;

    p_start = p_dst;
    p_end = (U8_T*)( p_dst + max );

    p_header = (PK16_HEADER_T*) p_pkg->p_buf;

    /* Ensure we can at least write the header, the data, and the packed tables */
    basic_len = sizeof(PK16_HEADER_T) + p_header->data_len + p_header->n * sizeof(PK16_TABLE_T);
    if (basic_len <= max) {
        /* Assume that we're OK until we see that we run out of space while 
         * expanding path strings. */
        result = PK16_OK;

        /* Write header */
        p_dst += memcpy(p_dst, (U8_T*) p_header, sizeof(PK16_HEADER_T));
        /* Write data */
        p_dst += memcpy(
            p_dst, 
            (U8_T*)( p_pkg->p_buf + sizeof(PK16_HEADER_T) ),
            p_header->data_len
        );

        /* Start writing tables until we either finish or run out of space */
        for (i = (SIZE_T) 0u; (PK16_FULL != result) && (i < p_header->n); ++i) {
            p_table = pk16_find_table_by_index(p_pkg, i);
            wrote_whole_path = FALSE;
            for (j = (SIZE_T) 0u; p_dst < p_end; ++j) {
                /* Copy the path string into the destination, including the \0 */
                p_dst = p_table->s_path[j];
                ++p_dst;
                if ('\0' == p_table->s_path[j]) {
                    wrote_whole_path = TRUE;
                    break;
                }
            }
            if (FALSE == wrote_whole_path) {
                /* We couldn't write the full path, buffer is full */
                result = PK16_FULL;
            } else if ((U8_T*)( 
                p_dst + sizeof(p_table->head) + sizeof(p_table->len) 
                + sizeof(p_table->checksum) ) > p_end) 
            {
                /* We can't write the rest of the table, buffer is full */
                result = PK16_FULL;
            } else {
                /* We can finish this table */
                p_dst += memcpy(p_dst, (U8_T*) &p_table->head, sizeof(p_table->head));
                p_dst += memcpy(p_dst, (U8_T*) &p_table->len, sizeof(p_table->len));
                p_dst += memcpy(p_dst, (U8_T*) &p_table->checksum, sizeof(p_table->checksum));
            }
        }
    }

    *final_len = (SIZE_T)( p_dst - p_start );

    return result;
}


/*******************************************************************************
 * Get a pointer to a table entry given its path in the table.
 * \param[out] p_pkg    The package in which to locate the table entry.
 * \param[in] s_path    The path of the table entry to be located.
 * \retval              A pointer to the located table entry.
 * \retval              `NULL` if the given path is not found.
 ******************************************************************************/
PK16_TABLE_T* pk16_find_table_by_path(PK16_T* p_pkg, CSTR_T s_path)
{
    PK16_HEADER_T* p_header;
    PK16_TABLE_T* p_table;
    SIZE_T i;
    
    p_table = (PK16_TABLE_T*) NULL;

    p_header = (PK16_HEADER_T*) p_pkg->p_buf;

    for (i = (SIZE_T) 0u; i < p_header->n; ++i) {
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
    U8_T* p_buf;
    PK16_TABLE_T* p_table;
    PK16_HEADER_T* p_header;

    p_header = (PK16_HEADER_T*) p_pkg->p_buf;

    p_buf = (U8_T*)( p_pkg->p_buf 
                    + sizeof(PK16_HEADER_T) 
                    + p_header->data_len 
                    + sizeof(PK16_TABLE_T) * index );

    if ( (U8_T*)(p_pkg->p_buf + p_pkg->size) > p_buf ) {
        p_table = (PK16_TABLE_T*) p_buf;
    } else {
        p_table = (PK16_TABLE_T*) NULL;
    }

    return p_table;
}