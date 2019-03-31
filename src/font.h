#ifndef FONT_H
#define FONT_H
#include "macro.h"
struct font {
	i32 n;
	i32 x;
	i32 y;
};

void font_init(void);
void print(i32 x, i32 y, i32 z, char *str);
#endif // FONT_H
