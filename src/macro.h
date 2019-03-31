#ifndef MACRO_H
#define MACRO_H

#include <stdint.h>
#include <stdlib.h>
#define NEW(type) (type *)calloc(1, sizeof(type))
#define MAKE(type, size) (type *)calloc(size, sizeof(type))
#define COUNT_OF(x)                                                            \
  ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

typedef uint8_t ui8;
typedef int8_t i8;
typedef float f32;
typedef double f64;
typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t ui32;
typedef uint64_t ui64;

#endif // MACRO_H
