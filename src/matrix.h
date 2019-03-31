#ifndef MATRIX_H
#define MATRIX_H
#include "macro.h"

void scale(f32 m[16], f32 x, f32 y, f32 z);
void ortho(f32 m[16], f32 left, f32 right, f32 bottom, f32 top, f32 nearval,
					 f32 farval);
void reset(f32 m[16]);
#endif // MATRIX_H
