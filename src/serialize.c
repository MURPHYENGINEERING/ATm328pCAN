#include "serialize.h"
#include "types.h"
#include "fai.h"


/*******************************************************************************
 * Serialize the given 32-bit integer into the given buffer in big-endian order.
 * \param[in] buf   The buffer into which the serialized value will be written.
 * \param[in] val   The 32-bit value to be serialized.
 * \return A pointer to the first position after the serialized data.
 ******************************************************************************/
U8_T* serialize_U32(U8_T* buf, U32_T val)
{
    buf[0] = (U8_T)( (val >> 24) & 0xFF );
    buf[1] = (U8_T)( (val >> 16) & 0xFF );
    buf[2] = (U8_T)( (val >> 8) & 0xFF );
    buf[3] = (U8_T)( val & 0xFF );

    return (U8_T*)( buf + 4u );
}

/*******************************************************************************
 * Serialize the given FAI fault into the given buffer in big-endian order.
 * \param[in] buf       The buffer into which the serialized value will be written.
 * \param[in] fault_id  The FAI fault ID to be serialized.
 * \param[in] fault     The FAI fault to be serialized.
 * \return A pointer to the first position after the serialized data.
 ******************************************************************************/
U8_T* serialize_fault(U8_T* buf, FAI_FAULT_ID_T fault_id, FAI_FAULT_COUNTER_T* fault)
{
    U8_T* p_buf;
    SIZE_T i;

    buf[0] = (U8_T) fault_id;
    buf[1] = (U8_T) fault->count;

    p_buf = &buf[2];
    for (i = 0; (i < fault->count) && (FAI_TS_DATA_LEN > i); ++i) {
        p_buf = serialize_U32(p_buf, fault->ts_data[i]);
    }
    
    return p_buf;
}

/*******************************************************************************
 * Deserialized a 16-bit integer from the given buffer in big-endian order.
 * \param[in] buf   The buffer from which the deserialized value will be read.
 * \return The deserialized 16-bit integer value.
 ******************************************************************************/
U16_T deserialize_U16(U8_T* buf)
{
    U16_T val;

    val = (U16_T) 0;
    val |= (U16_T) buf[0] << 8;
    val |= (U16_T) buf[1];

    return val;
}

/*******************************************************************************
 * Deserialized a CAN message identifier integer from the given buffer in 
 * big-endian order.
 * \param[in] buf   The buffer from which the deserialized value will be read.
 * \return The deserialized CAN identifier.
 ******************************************************************************/
CAN_IDENT_T deserialize_can_identifier(U8_T* buf)
{
    return (CAN_IDENT_T) deserialize_U16(buf);
}