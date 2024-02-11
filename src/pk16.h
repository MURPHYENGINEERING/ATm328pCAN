#ifndef __PK16_H__
#define __PC16_H__

#include "types.h"

#define PK16_MAX_PATH_LEN 256

/** Represents the result of an operation on a PK16 package. */
typedef enum {
    /** The operation completed successfully. */
    PK16_OK,
    /** The package is full and the operation cannot complete. */
    PK16_FULL,
    /** The package is empty and the operation cannot complete. */
    PK16_EMPTY,
    /** The package already contains ane entry at that path. */
    PK16_EXISTS,
    /** The given data buffer does not contain a PK16 package. */
    PK16_NOT_A_PACKAGE,
    /** The given PK16 package is of the wrong version. */
    PK16_WRONG_VERSION
} PK16_RESULT_T;


/** A single entry in a PK16 package. */
typedef struct {
    /** Null-terminated string representing the path of this entry. */
    CSTR_T s_path;
    /** Index into the package data buffer of the start of this entry's data. */
    U16_T head;
    /** Length in bytes of this entry's data in the data buffer. */
    U16_T len;
    /** Checksum of this entry's data. */
    U32_T checksum;
} PK16_TABLE_T;


/** A header for a PK16 package. */
typedef struct {
    /** Magic number indicating this is the correct package format. */
    U16_T magic;
    /** Package version for compatibility over revisions. */
    U8_T version;
    /** Number of entries in the package table. */
    U16_T n;
    /** Length of the data in the package, used to offset to the package table. */
    U16_T data_len;
} PK16_HEADER_T;


/** A PK16 package with its header and underlying buffer information. */
typedef struct {
    /** Pointer to the underlying data buffer in which the package is stored. */
    U8_T* p_buf;
    /** Size in bytes of the underlying data buffer; i.e., the maximum length of
      * the package data + header. */
    SIZE_T size;
} PK16_T;


void pk16_init(PK16_T* p_pkg, U8_T* p_buf, SIZE_T size);
PK16_RESULT_T pk16_add(PK16_T* p_pkg, CSTR_T s_path, U8_T* p_data, SIZE_T len);
PK16_RESULT_T pk16_remove(PK16_T* p_pkg, CSTR_T s_path);

PK16_RESULT_T pk16_serialize(PK16_T* p_pkg, U8_T* p_dst, SIZE_T* final_len, SIZE_T max);

SIZE_T pk16_read(PK16_T* p_pkg, CSTR_T s_path, U8_T* p_dst, SIZE_T max);

PK16_TABLE_T* pk16_find_table_by_index(PK16_T* p_pkg, SIZE_T index);
PK16_TABLE_T* pk16_find_table_by_path(PK16_T* p_pkg, CSTR_T s_path);


#endif