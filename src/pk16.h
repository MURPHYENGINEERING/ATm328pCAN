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
    /** String representing the path of this entry. */
    U8_T path[64];
    /** Length in bytes of the `path` of this entry. */
    U16_T path_len;
    /** Index into the package data buffer of the start of this entry's data. */
    U16_T head;
    /** Length in bytes of this entry's data in the data buffer. */
    U16_T len;
    /** CRC32 of this entry's path + data. */
    U32_T crc;
} PK16_TABLE_T;


/** A header to a PK16 package. */
typedef struct {
    U16_T magic;
    U8_T version;
    U16_T n;
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