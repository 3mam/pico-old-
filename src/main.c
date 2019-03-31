#include "font.h"
#include "input.h"
#include "time.h"
#include "graphic.h"

int main(int argc, char *argv[]) {
	draw_init(320, 180);
	font_init();
	input_init();

	/*
	if (argc > 1) {
		if (strcmp(argv[1], "paint") == 0) {
			//   paint_init();
			while (1) {
				//     paint();
			}
		}
	}
	*/
	delta *time = delta_init();

	i32 p[] = { 0, 1 };
	f64 f = 0.0;
	//sprite *mag = spr_load("data/mag.png");
	sprite *level = spr_load("data/lvl2.png");
	sprite *player = spr_load("data/player.png");
	f64 a = 0.0;
	while (1) {
		delta_start(time);
		cls(1);

		spr(level, 0, 0, 0, 0, 0, 0, 0);

		//spr(mag, p[(i32)f], 20, 20, 1, 1, 0, 0);
		spr(player, p[(i32)f], 150, 100, 1, 2, 0, 0);
		//print(10, 10, 0, "ABCDEFGHIJKLMNOPRSTUWYZ\nabcdefghijklmnoprstuwyz\n0123456789,.");
		//circ_fill(10, 11, 4, 8, 50, 0);
		f += time->delta * 0.8;
		if (f > 2) {
			f = 0.0;
		}
		rect_fill(10, 10, 30, 30, 21, 255, 0);
		    a +=  time->delta * 60;
    if (a > 320) {
      a = 0;
    }
		circ_fill(a, 50, 10, 22, 255, 0);
		draw();
		delta_end(time);
	}
	return 0;
}