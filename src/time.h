#ifndef TIME_C
#define TIME_C

#include "macro.h"
#include <SDL2/SDL.h>

typedef struct {
	ui64 now;
	ui64 then;
	f64 delta;
} delta;

delta *delta_init(void);
void delta_start(delta *d);
void delta_end(delta *d);

#endif // TIME_C
