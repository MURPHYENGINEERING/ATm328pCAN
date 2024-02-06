#include "pk16.h"
#include "types.h"
#include "memory.h"
#include "string.h"
#include "crc.h"



/*******************************************************************************
 * Initialize the given package with the given underlying buffer.
 * \param[out] p_pkg  The package to initialize to the given buffer.
 * \param[in] buf   The underlying buffer the package (with header) will be 
 *                  written into.
 * \param[in] len   The length in bytes of the underlying buffer.
 ******************************************************************************/
void pk16_init(PK16_T* p_pkg, U8_T* buf, SIZE_T len)
{
    memset(p_pkg, (U8_T) 0u, sizeof(PK16_T));

    p_pkg->header.magic = (U16_T) 0xBEEFu;
    p_pkg->header.version = (U8_T) 1u;

    p_pkg->buf = buf;
    p_pkg->size = len;

    memset(buf, (U8_T) 0u, len);
}



/*******************************************************************************
 * Add an entry into the given PK16 package. 
 * \param[out] p_pkg  The package into which to add an entry.
 * \param[in] path  The path at which the given entry can be found.
 * \param[in] data  The contents of the entry.
 * \param[in] len   The length in bytes of the `data` array.
 * \retval          `PK16_OK` if the entry was added.
 * \retval          `PK16_FULL` if the package is full.
 ******************************************************************************/
PK16_RESULT_T pk16_add(PK16_T* p_pkg, CSTR_T path, U8_T* data, SIZE_T len)
{
    PK16_RESULT_T result;
    PK16_TABLE_T* p_table; 
    SIZE_T old_table_head;
    SIZE_T new_table_head;

    result = PK16_FULL;

    /* If the size of the header + the existing data + this new data
     * + the existing table entries + this new table entry doesn't exceed the
     * underlying buffer size... */
    if ((sizeof(PK16_HEADER_T) 
        + p_pkg->header.data_len 
        + len 
        + sizeof(PK16_TABLE_T) * (p_pkg->header.n + 1)
        ) < pkg->size)
    {
        /* Locate where the table is and where it should be after the new data
         * are added */
        old_table_head = (SIZE_T)( sizeof(PK16_HEADER_T) + pkg->header.data_len );
        new_table_head = (SIZE_T)( old_table_head + len );

        /* Move the entire table to make room for the new data */
        memcpy(
            &pkg->buf[new_table_head], 
            &pkg->buf[old_table_head], 
            sizeof(PK16_TABLE_T) * pkg->header.n
        );

        /* Copy in the new data */
        memcpy(&pkg->buf[old_table_head], data, len);
        /* Add the new data to the header */
        pkg->header.data_len += len;

        /* Write the new table entry into the end of the table */
        p_table = pk16_find_table_by_index(pkg, pkg->header.n);
        strncpy(p_table->path, path, PK16_MAX_PATH_LEN);
        /* The new entry's data goes where the table was */
        p_table->head = old_table_head;
        p_table->len = len;
        p_table->crc = crc_compute_crc32(data, len, (U32_T) 0u, TRUE);

        /* Add the new table entry to the header */
        ++pkg->header.n;

        result = PK16_OK;
    }

    return result;
}


/*******************************************************************************
 * Remove an entry from the given PK16 package according to its index in the table.
 * \param[out] pkg  The package from which to remove an entry.
 * \param[in] index The index of the entry to be removed.
 * \retval          `PK16_OK` if the entry was removed.
 * \retval          `PK16_EMPTY` if the indexed entry doesn't exist.
 ******************************************************************************/
PK16_RESULT_T pk16_remove_by_index(PK16_T* pkg, SIZE_T index)
{
    PK16_RESULT_T result;
    PK16_TABLE_T* p_table; 
    SIZE_T removed_len;
    SIZE_T old_table_head;
    SIZE_T new_table_head;
    SIZE_T old_data_head;
    SIZE_T trailing_data_len;
    SIZE_T new_data_head;
    SIZE_T i;

    result = PK16_EMPTY;

    if (index < pkg->header.n) {
        /* Locate the table entry for this index */
        p_table = pk16_find_table_by_index(pkg, index);

        removed_len = p_table->len;

        old_data_head = (SIZE_T)( p_table->head + removed_len );
        trailing_data_len = (SIZE_T)( pkg->header.len - old_data_head );
        new_data_head = (SIZE_T)( old_data_head - removed_len );

        /* Move the trailing data up to fill in the cleared space. */
        memcpy(&pkg->buf[new_data_head], &pkg->buf[old_data_head], trailing_data_len);

        pkg->header.data_len -= removed_len;

        /* Rewrite every table entry that isn't the removed one onto the end of
         * the data. */
         new_table_head = sizeof(PK16_HEADER_T) + pkg->header.data_len;

         for (i = 0; i < pkg->header.n; ++i) {
            if (i != index) {
                p_table = pk16_find_table_by_index(i);
                /* Update the trailing table entries with the new data heads */
                if (i > index) {
                   p_table->head -= removed_len;
                }
                memcpy(&pkg->buf[new_table_head], p_table, sizeof(PK16_TABLE_T));
                new_table_head += sizeof(PK16_TABLE_T);
            }
         }

         --pkg->header.n;

        result = PK16_OK;
    }

    return result;
}


/*******************************************************************************
 * Read the specified entry data from the package.
 * \param[in] pkg   The package to read from.
 * \param[in] path  The path of the entry to read.
 * \param[out] dst  The buffer into which to read the data.
 * \param[in] max   The maximum number of bytes to read.
 * \return          The number of bytes read.
 ******************************************************************************/
SIZE_T pk16_read(PK16_T* pkg, CSTR_T path, U8_T* dst, SIZE_T max)
{
    SIZE_T i;
    PK16_TABLE_T* p_table;

    for (i = 0; i < pkg->header.n; ++i) {

    }
}


/*******************************************************************************
 * Get a pointer to the table entry given its index in the table.
 * \param[out] p_pkg  The package in which to locate the table entry.
 * \param[in] index The index of the table entry to be located.
 * \retval          A pointer to the located table entry.
 * \retval          `NULL` if the given index is out of range.
 ******************************************************************************/
PK16_TABLE_T* pk16_find_table_by_index(PK16_T* p_pkg, SIZE_T index)
{
    SIZE_T offset;
    PK16_TABLE_T* p_table;

    offset = (SIZE_T)( sizeof(PK16_HEADER_T) 
                        + p_pkg->header.data_len 
                        + sizeof(PK16_TABLE_T) * index );

    if (offset < (p_pkg->size - sizeof(PK16_TABLE_T))) {
        p_table = (PK16_TABLE_T*)(void*) &p_pkg->buf[offset];
    } else {
        p_table = (PK16_TABLE_T*) NULL;
    }

    return p_table;
}