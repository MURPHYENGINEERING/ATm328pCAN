#ifndef __SERIALIZE_H__
#define __SERIALIZE_H__

#include "types.h"
#include "fai.h"

U8_T* serialize_U32(U8_T* buf, U32_T val);
U8_T* serialize_fault(U8_T* buf, FAI_FAULT_ID_T fault_id, FAI_FAULT_T* fault);

U16_T deserialize_U16(U8_T* buf);
CAN_IDENT_T deserialize_can_identifier(U8_T* buf);

#endif