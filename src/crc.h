#ifndef __CRC_H__
#define __CRC_H__

#include "types.h"


/** CRC-32 table with polynomial 0xEDB88320 */
extern const U32_T g_crc_crc32_tbl[256u];

/** CRC-16 Polynomial Implementation */
#define CRC_UPD_CRC16(halfword, crc) \
    (g_crc_crc16_tbl[(U16_T)((U16_T)((crc) >> 8) & 0xFFu)] ^ (U16_T)((crc) << 8) ^ (halfword))

/** CRC-32 Polynomial Implementation */
#define CRC_UPD_CRC32(octet, crc) \
    (g_crc_crc32_tbl[((crc) ^ (octet)) & 0xFFu] ^ ((crc) >> 8))

U8_T crc_compute_checksum(U8_T* buf, SIZE_T len, U8_T running_val);
U16_T crc_compute_checksum16(U8_T* buf, SIZE_T len, U16_T running_val);
U32_T crc_compute_checksum32(U8_T* buf, SIZE_T len, U32_T running_val);
U16_T crc_compute_crc16(U8_T* buf, SIZE_T len, U16_T running_val, BOOL_T final);
U32_T crc_compute_crc32(void* buf, SIZE_T len, U32_T running_val, BOOL_T final);

#endif