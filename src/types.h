#ifndef __TYPES_H__
#define __TYPES_H__

typedef _Bool BOOL_T;
typedef volatile BOOL_T VBOOL_T;


#ifdef ATM328P
    typedef char S8_T;
    typedef unsigned char U8_T;
    typedef volatile U8_T VU8_T;
    #define U8_T_MAX ((U8_T) 255u)

    typedef int S16_T;
    typedef unsigned short U16_T;
    typedef volatile U16_T VU16_T;
    #define U16_T_MAX ((U16_T) 65535u)

    typedef long S32_T;
    typedef unsigned long U32_T;
    typedef volatile U32_T VU32_T;
    #define U32_T_MAX ((U32_T) 4294967295u)

    typedef U16_T SIZE_T;
    typedef volatile SIZE_T VSIZE_T;
    #define SIZE_T_MAX U16_T_MAX
#endif

typedef char CHAR_T;
typedef CHAR_T* CSTR_T;

typedef float FLOAT_T;
/** Approximately zero equals zero. */
#define ZERO_EPSILON 0.0001f
#define EQUALS_ZERO(x) (BOOL_T)( (-ZERO_EPSILON < x) && (ZERO_EPSILON > x) )

/** Represents a hardware register's bits, or its total byte value. */
typedef union {
    struct {
        VBOOL_T bit0 : 1;
        VBOOL_T bit1 : 1;
        VBOOL_T bit2 : 1;
        VBOOL_T bit3 : 1;
        VBOOL_T bit4 : 1;
        VBOOL_T bit5 : 1;
        VBOOL_T bit6 : 1;
        VBOOL_T bit7 : 1;
    } bits;
    VU8_T byte;
} REGISTER_T;


/** Represents a 16-bit hardware register with high and low bytes. */
typedef union {
    struct {
        volatile REGISTER_T low;
        volatile REGISTER_T high;
    } bytes;
    VU8_T data[2];
    VU16_T halfword;
} HALFWORD_T;


/** Represents a 32-bit hardware register comprising four bytes. */
typedef union {
    struct {
        volatile REGISTER_T byte0;
        volatile REGISTER_T byte1;
        volatile REGISTER_T byte2;
        volatile REGISTER_T byte3;
    } bytes;
    struct {
        VU16_T low;
        VU16_T high;
    } halfwords;
    VU8_T data[4];
    VU32_T word;
} WORD_T;


#define TRUE  (BOOL_T) 1u
#define FALSE (BOOL_T) 0u


typedef enum {
    LOW = 0u,
    GND = LOW,
    HIGH = 1u,
    OPEN = HIGH
} BIT_T;


typedef enum {
    OFF = 0u,
    ON = 1u
} ON_T;


typedef enum {
    DISABLED = 0u,
    ENABLED = 1u
} ENABLE_T;


typedef enum {
    FAIL = 0u,
    PASS = 1u
} PASS_T;


typedef enum {
    INVALID = 0u,
    VALID = 1u
} VALID_T;


typedef enum {
    DDR_INPUT = LOW,
    DDR_OUTPUT = HIGH
} DDR_T;


typedef enum {
    PORT_PULLDOWN = LOW,
    PORT_PULLUP = HIGH
} PORT_T;


#define NULL (void*) 0u


#endif