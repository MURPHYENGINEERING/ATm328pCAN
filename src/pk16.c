#include "pk16.h"
#include "types.h"
#include "memory.h"
#include "string.h"
#include "crc.h"
#include "usart.h"


/** Magic number identifying an array of bytes as a PK16 package. */
#define PK16_MAGIC (U16_T) 0xEFBEu
/** Current version of this PK16 library. */
#define PK16_VERSION (U8_T) 1u


static CSTR_T pk16_get_path(PK16_TABLE_T* p_table);
static PK16_TABLE_T* pk16_find_table_by_index(PK16_T* p_pkg, SIZE_T index);
static PK16_TABLE_T* pk16_find_table_by_path(PK16_T* p_pkg, CSTR_T s_path);


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
    p_header->data_len = (SIZE_T) 0u;
    p_header->n = (SIZE_T) 0u;
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
    U8_T* p_old_table_head;
    U8_T* p_old_table_tail;
    U8_T* p_new_table_tail;
    PK16_TABLE_T* p_table;
    SIZE_T i;
    SIZE_T tables_len;
    
    p_header = (PK16_HEADER_T*) p_pkg->p_buf;

    result = PK16_FULL;

    if (PK16_MAGIC != p_header->magic) {
        result = PK16_NOT_A_PACKAGE;
    } else if (PK16_VERSION != p_header->version) {
        result = PK16_WRONG_VERSION;
    } else if (sizeof(PK16_HEADER_T) + p_header->data_len + len + sizeof(PK16_TABLE_T) * (p_header->n + 1)) {
        /* Locate where the table is and where it should be after the new data are added */
        
        /* The table is currently located at the end of the header and data. */
        p_old_table_head = (U8_T*)( p_pkg->p_buf + sizeof(PK16_HEADER_T) + p_header->data_len );

        /* Start the table tail at the table head. For an empty table, these are the same. */
        p_old_table_tail = p_old_table_head;

        /* Find the end of the tables by stepping over the paths */
        for (i = (SIZE_T) 0u; i < p_header->n; ++i) {
            p_old_table_tail += sizeof(PK16_TABLE_T) + ((PK16_TABLE_T*)p_old_table_tail)->path_len;
        }

        /* This is where we'll move the end of the table to */
        p_new_table_tail = p_old_table_tail + len;
        p_table = (PK16_TABLE_T*) p_new_table_tail;

        /* Move the existing tables back by `len` bytes */
        tables_len = p_old_table_tail - p_old_table_head;
        for (i = (SIZE_T) 0u; i < tables_len; ++i) {
            *(p_new_table_tail - 1 - i) = *(p_old_table_tail - 1 - i);
        }

        /* Set up the table header */
        p_table->data_head = sizeof(PK16_HEADER_T) + p_header->data_len;
        p_table->data_len = len;
        p_table->path_len = strnlen(s_path, PK16_MAX_PATH_LEN) + 1;
        p_table->checksum = crc_compute_checksum32(p_data, len, (U32_T) 0u);
        /* Copy in the path at the end of the table header */
        strncpy((CSTR_T)( p_table + 1u ), s_path, p_table->path_len);

        /* Copy in the data */
        memcpy(p_pkg->p_buf + sizeof(PK16_HEADER_T) + p_header->data_len, p_data, len);

        /* Add the entry to the package header */
        p_header->data_len += len;
        ++p_header->n;

        result = PK16_OK;
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
    SIZE_T removed_len;
    SIZE_T trailing_len;
    BOOL_T relocate_heads;
    SIZE_T i;

    result = PK16_EMPTY;

    p_header = (PK16_HEADER_T*) p_pkg->p_buf;
    p_table = (PK16_TABLE_T*) NULL;

    if (PK16_MAGIC != p_header->magic) {
        result = PK16_NOT_A_PACKAGE;
    } else if (PK16_VERSION != p_header->version) {
        result = PK16_WRONG_VERSION;
    } else if ((SIZE_T) 0u < p_header->n) {
        p_table = pk16_find_table_by_path(p_pkg, s_path);
    }

    if (NULL != p_table) {
        removed_len = p_table->data_len;

        /* Locate the remaining data before and after the removed data */
        p_cursor = &p_pkg->p_buf[p_table->data_head];
        p_trailing = (U8_T*)( p_cursor + removed_len);
        trailing_len = sizeof(PK16_HEADER_T)
                     + p_header->data_len
                     - p_table->data_head
                     - removed_len;

        /* Move the data after the removed data up by `len` bytes */
        for (i = (SIZE_T) 0u; i < trailing_len; ++i) {
            *p_cursor = *p_trailing;
            ++p_cursor;
            ++p_trailing;
        }

        /* Point the write cursor to the new location for the table */
        p_cursor = (U8_T*)(
            p_pkg->p_buf + sizeof(PK16_HEADER_T) + p_header->data_len - removed_len
        );

        /* Don't update table heads until we get to the data after the removed data. */
        relocate_heads = FALSE;
        for (i = (SIZE_T) 0u; i < p_header->n; ++i) {
            p_table = pk16_find_table_by_index(p_pkg, i);
            /* Skip the removed table entry */
            if ( 0 != strncmp(pk16_get_path(p_table), s_path, PK16_MAX_PATH_LEN) ) {
                if (TRUE == relocate_heads) {
                    /* This table's data moved up by `removed_len` */
                    p_table->data_head -= removed_len;
                }
                /* Copy the whole table including the path */
                memcpy(p_cursor, (U8_T*) p_table, sizeof(PK16_TABLE_T) + p_table->path_len);
                /* Move the write cursor to the next table entry */
                p_cursor += sizeof(PK16_TABLE_T) + p_table->path_len;
            } else {
                /* Mark all following tables as needing their data head updated */
                relocate_heads = TRUE;
            }
        }

        /* Remove the entry from the table header */
        p_header->data_len -= removed_len;
        --p_header->n;

        result = PK16_OK;
    }

    return result;
}


/*******************************************************************************
 * Read the data for the specified path from the package.
 * \param[in] pkg       The package to read from.
 * \param[in] s_path    The path of the entry to read.
 * \param[out] dst      The buffer into which to read the data.
 * \param[in] max       The maximum number of bytes to read.
 * \return              The number of bytes read.
 ******************************************************************************/
SIZE_T pk16_read(PK16_T* p_pkg, CSTR_T s_path, U8_T* p_dst, SIZE_T max)
{
    SIZE_T bytes_read;
    PK16_TABLE_T* p_table;
    U32_T checksum;

    bytes_read = (SIZE_T) 0u;

    p_table = pk16_find_table_by_path(p_pkg, s_path);

    if (NULL != p_table) {
        checksum = crc_compute_checksum32(
                        &p_pkg->p_buf[p_table->data_head], 
                        p_table->data_len, 
                        (U32_T) 0u
                    );
        if (checksum == p_table->checksum) {
            if (max > p_table->data_len) {
                max = p_table->data_len;
            }
            memcpy(p_dst, &p_pkg->p_buf[p_table->data_head], max);
            bytes_read = max;
        }
    }

    return bytes_read;
}



/*******************************************************************************
 * Get a pointer to a table entry given its path in the table.
 * \param[out] p_pkg    The package in which to locate the table entry.
 * \param[in] s_path    The path of the table entry to be located.
 * \retval              A pointer to the located table entry.
 * \retval              `NULL` if the given path is not found.
 ******************************************************************************/
static PK16_TABLE_T* pk16_find_table_by_path(PK16_T* p_pkg, CSTR_T s_path)
{
    PK16_HEADER_T* p_header;
    PK16_TABLE_T* p_table;
    SIZE_T i;
    
    p_table = (PK16_TABLE_T*) NULL;

    p_header = (PK16_HEADER_T*) p_pkg->p_buf;

    for (i = (SIZE_T) 0u; i < p_header->n; ++i) {
        p_table = pk16_find_table_by_index(p_pkg, i);
        if (0 == strncmp(s_path, pk16_get_path(p_table), PK16_MAX_PATH_LEN)) {
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
static PK16_TABLE_T* pk16_find_table_by_index(PK16_T* p_pkg, SIZE_T index)
{
    U8_T* p_table;
    PK16_HEADER_T* p_header;
    SIZE_T i;

    p_header = (PK16_HEADER_T*) p_pkg->p_buf;
    p_table = (U8_T*) ( p_pkg->p_buf + sizeof(PK16_HEADER_T) + p_header->data_len );

    for (i = (SIZE_T) 0u; i < index; ++i) {
        /* Skip the table header + path to point to the next table header */
        p_table += sizeof(PK16_TABLE_T) + ((PK16_TABLE_T*)p_table)->path_len;
    } 
    
    return (PK16_TABLE_T*) p_table;
}


/*******************************************************************************
 * Get a pointer to the null-terminated path string for the given table entry.
 * \param[in] p_table A pointer to the given table entry.
 * \return A pointer to the null-terminated path string for the given table entry.
 ******************************************************************************/
static CSTR_T pk16_get_path(PK16_TABLE_T* p_table)
{
    return (CSTR_T)( p_table + 1u );
}