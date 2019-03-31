#include "time.h"

void delta_start(delta *d) { d->then = SDL_GetPerformanceCounter(); }

void delta_end(delta *d) {
	d->now = SDL_GetPerformanceCounter();
	d->delta = (double)(d->now - d->then);
	d->delta /= (double)(SDL_GetPerformanceFrequency());
}

delta *delta_init(void) {
	delta *d = NEW(delta);
	d->delta = 0.016;
	return d;
}