#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "macro.h"
#include "sprite.h"

void draw_init(i32 width, i32 height);
void camera(i32 x, i32 y);
void draw(void);
void line(i32 x0, i32 y0, i32 x1, i32 y1, i32 col, i32 a, i32 z);
void rect(i32 x0, i32 y0, i32 x1, i32 y1, i32 col, i32 a, i32 z);
void rect_fill(i32 x0, i32 y0, i32 x1, i32 y1, i32 col, i32 a, i32 z);
void circ(i32 x0, i32 y0, i32 r, i32 col, i32 a, i32 z);
void circ_fill(i32 x0, i32 y0, i32 r, i32 col, i32 a, i32 z);
void tri(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, i32 col, i32 a, i32 z);
void tri_fill(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, i32 col, i32 a,
	i32 z);

// void spr(sprite *s, i32 frame, i32 dx, i32 dy, i32 dw, i32 dh, i32 flip, i32
// a, 				 i32 z); void spr2(sprite *s, i32 dx, i32 dy);

sprite *spr_load(char *name);
void spr(sprite *s, i32 n, i32 x, i32 y, i32 w, i32 h, i32 flip, i32 z);
void cls(i32 col);
void pal(i32 c0, i32 c1, i32 t);
void palc(void);
void palt(i32 c, i32 t);
void clip(i32 x, i32 y, i32 w, i32 h);
i32 pget(i32 x, i32 y);
void pset(i32 x, i32 y, i32 c, i32 a, i32 z);

void get_screen_size(i32 *w, i32 *h);

#endif // GRAPHIC_H
