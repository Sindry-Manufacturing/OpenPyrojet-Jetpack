#ifndef OPENPYROJET_ALLOC_H
#define OPENPYROJET_ALLOC_H

#include <stdlib.h>

#define malloc_one(_type) (_type*)malloc(sizeof(_type))
#define malloc_array(_type, _count) (_type*)malloc(sizeof(_type) * _count)

#endif // OPENPYROJET_ALLOC_H
