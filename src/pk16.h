#ifndef __PK16_H__
#define __PC16_H__

#include "types.h"


/** A header to a Package-16 package. */
typedef struct {
    U16_T magic;
    U16_T len;
} PK16_HEADER_T;


/** A single entry in a Package-16 package. */
typedef struct {
    /** String representing the path of this entry. */
    U8_T* path;
    /** Length in bytes of the `path` of this entry. */
    U16_T path_len;
    /** Index into the package data buffer of the start of this entry. */
    U16_T head;
    /** Length in bytes of this entry's data in the data buffer. */
    U16_T len;
    /** CRC32 of this entry's path + data. */
    U32_T crc;
    PK16_ENTRY_T* next;
} PK16_ENTRY_T;


/** Table of Package-16 entry metadata. */
typedef struct {
    PK16_ENTRY_T head;
    U16_T n;
} PK16_TABLE_T;


/** A Package-16 package with its header and underlying buffer information. */
typedef struct {
    /** The Package-16 package header. */
    PK16_HEADER_T header;
    /** Pointer to the underlying data buffer in which the package is stored. */
    U8_T* buf;
    /** Size in bytes of the underlying data buffer; i.e., the maximum length of
      * the package data + header. */
    SIZE_T size;
    /** Index into the underlying buffer where the next write will occur. */
    SIZE_T head;
} PK16_T;


void pk16_init(PK16_T* pkg, U8_T* buf, SIZE_T len);
void pk16_add(PK16_T* pkg, CSTR_T path, U8_T* data, SIZE_T len);

#endif