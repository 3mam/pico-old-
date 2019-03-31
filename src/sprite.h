#ifndef SPRITE_H
#define SPRITE_H
#include "macro.h"
#include <SDL2/SDL.h>
typedef struct sprite_sector {
	i32 x;
	i32 y;
} sprite_sector;

typedef struct sprite {
	i32 w;
	i32 h;
	i32 n;
	ui8 *canvas;
	sprite_sector *cell;
} sprite;

/*extern atlas sprite;*/
#endif // SPRITE_H
