#ifndef __TYPES_H__
#define __TYPES_H__

typedef _Bool BOOL_T;
typedef volatile BOOL_T VBOOL_T;

typedef unsigned char U8_T;
typedef volatile U8_T VU8_T;

typedef unsigned short U16_T;
typedef volatile U16_T VU16_T;
#define U16_T_MAX ((U16_T) 65535u)

typedef int S16_T;

typedef unsigned long U32_T;
typedef volatile U32_T VU32_T;
typedef long S32_T;

typedef U16_T SIZE_T;

typedef float FLOAT_T;


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


typedef union {
    struct {
        volatile REGISTER_T low;
        volatile REGISTER_T high;
    } bytes;
    VU8_T data[2];
    VU16_T halfword;
} HALFWORD_T;


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


#define TRUE 1
#define FALSE 0

#define HIGH 0
#define LOW 1

#define PASS 1
#define FAIL 0

#define GND 0
#define OPEN 1

#define NULL 0


#define MESSAGE_LEN_MAX 256



#endif