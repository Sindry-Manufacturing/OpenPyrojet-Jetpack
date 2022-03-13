#ifndef OPENPYROJET_TYPES_H
#define OPENPYROJET_TYPES_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t byte;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef float float32;
typedef double float64;

#define mallocStruct(_type) (struct _type*)malloc(sizeof(struct _type))
#define mallocStructArray(_type, _count) (struct _type*)malloc(sizeof(struct _type) * _count)

#endif // OPENPYROJET_TYPES_H
