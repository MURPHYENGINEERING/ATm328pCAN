#ifndef __PK16_H__
#define __PC16_H__

#include "types.h"

#define PK16_MAX_PATH_LEN 64

/** Represents the result of an operation on a PK16 package. */
typedef enum {
    /** The operation completed successfully. */
    PK16_OK,
    /** The package is full and the operation cannot complete. */
    PK16_FULL,
    /** The package is empty and the operation cannot complete. */
    PK16_EMPTY
} PK16_RESULT_T;


/** A single entry in a PK16 package. */
typedef struct {
    /** Null-terminated string representing the path of this entry. */
    S8_T path[PK16_MAX_PATH_LEN];
    /** Index into the package data buffer of the start of this entry's data. */
    U16_T head;
    /** Length in bytes of this entry's data in the data buffer. */
    U16_T len;
    /** CRC32 of this entry's path + data. */
    U32_T crc;
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
    /** The PK16 package header. */
    PK16_HEADER_T header;
    /** Pointer to the underlying data buffer in which the package is stored. */
    U8_T* buf;
    /** Size in bytes of the underlying data buffer; i.e., the maximum length of
      * the package data + header. */
    SIZE_T size;
} PK16_T;


void pk16_init(PK16_T* pkg, U8_T* buf, SIZE_T len);
PK16_RESULT_T pk16_add(PK16_T* pkg, CSTR_T path, U8_T* data, SIZE_T len);


#endif