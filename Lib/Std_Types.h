/* Common types, following the stm32f4-sec10 driver convention. */
#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <stdint.h>

typedef int8_t   sint8;
typedef uint8_t  uint8;
typedef int16_t  sint16;
typedef uint16_t uint16;
typedef int32_t  sint32;
typedef uint32_t uint32;
typedef int64_t  sint64;
typedef uint64_t uint64;
typedef float    float32;
typedef double   float64;
typedef uint8   boolean;

#define FALSE ((boolean)0)
#define TRUE  ((boolean)1)

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif
