#include "font.h"
#include "graphic.h"
#include "sprite.h"
#include <string.h>
static struct font _A = {
		0,
		6,
		0,
};

static struct font _B = {
		1,
		6,
		0,
};
static struct font _C = {
		2,
		6,
		0,
};
static struct font _D = {
		3,
		6,
		0,
};
static struct font _E = {
		4,
		5,
		0,
};
static struct font _F = {
		5,
		5,
		0,
};
static struct font _G = {
		6,
		6,
		0,
};
static struct font _H = {
		7,
		6,
		0,
};
static struct font _I = {
		8,
		4,
		0,
};
static struct font _J = {
		9,
		5,
		0,
};
static struct font _K = {
		10,
		6,
		0,
};
static struct font _L = {
		11,
		5,
		0,
};
static struct font _M = {
		12,
		6,
		0,
};
static struct font _N = {
		13,
		6,
		0,
};
static struct font _O = {
		14,
		6,
		0,
};
static struct font _P = {
		15,
		6,
		0,
};
static struct font _R = {
		16,
		6,
		0,
};
static struct font _S = {
		17,
		6,
		0,
};
static struct font _T = {
		18,
		6,
		0,
};
static struct font _U = {
		19,
		6,
		0,
};
static struct font _W = {
		20,
		6,
		0,
};
static struct font _Y = {
		21,
		5,
		0,
};
static struct font _Z = {
		22,
		6,
		0,
};
static struct font _a = {
		23,
		6,
		0,
};
static struct font _b = {
		24,
		6,
		0,
};
static struct font _c = {
		25,
		6,
		0,
};
static struct font _d = {
		26,
		6,
		0,
};
static struct font _e = {
		27,
		6,
		0,
};
static struct font _f = {
		28,
		5,
		0,
};
static struct font _g = {
		29,
		6,
		0,
};
static struct font _h = {
		30,
		6,
		0,
};
static struct font _i = {
		31,
		2,
		0,
};
static struct font _j = {
		32,
		3,
		0,
};
static struct font _k = {
		33,
		6,
		0,
};
static struct font _l = {
		34,
		2,
		0,
};
static struct font _m = {
		35,
		8,
		0,
};
static struct font _n = {
		36,
		5,
		0,
};
static struct font _o = {
		37,
		6,
		0,
};
static struct font _p = {
		38,
		6,
		0,
};
static struct font _r = {
		39,
		5,
		0,
};
static struct font _s = {
		40,
		6,
		0,
};
static struct font _y = {
		44,
		6,
		0,
};
static struct font _u = {
		42,
		6,
		0,
};
static struct font _w = {
		43,
		6,
		0,
};
static struct font _t = {
		41,
		6,
		0,
};
static struct font _z = {
		45,
		6,
		0,
};
static struct font _0 = {
		65,
		4,
		0,
};
static struct font _1 = {
		66,
		5,
		0,
};
static struct font _2 = {
		67,
		5,
		0,
};
static struct font _3 = {
		68,
		5,
		0,
};
static struct font _4 = {
		69,
		5,
		0,
};
static struct font _5 = {
		70,
		5,
		0,
};
static struct font _6 = {
		71,
		5,
		0,
};
static struct font _7 = {
		72,
		5,
		0,
};
static struct font _8 = {
		73,
		5,
		0,
};
static struct font _9 = {
		74,
		5,
		0,
};

static struct font _dot = {
		75,
		2,
		0,
};

static struct font _comma = {
		76,
		2,
		0,
};

static struct font *font[128];
static sprite *img;

void font_init(void) {
	img = spr_load("data/font.png");
	font['A'] = &_A;
	font['B'] = &_B;
	font['C'] = &_C;
	font['D'] = &_D;
	font['E'] = &_E;
	font['F'] = &_F;
	font['G'] = &_G;
	font['H'] = &_H;
	font['I'] = &_I;
	font['J'] = &_J;
	font['K'] = &_K;
	font['L'] = &_L;
	font['M'] = &_M;
	font['N'] = &_N;
	font['O'] = &_O;
	font['P'] = &_P;
	font['R'] = &_R;
	font['S'] = &_S;
	font['T'] = &_T;
	font['U'] = &_U;
	font['W'] = &_W;
	font['Y'] = &_Y;
	font['Z'] = &_Z;
	font['a'] = &_a;
	font['b'] = &_b;
	font['c'] = &_c;
	font['d'] = &_d;
	font['e'] = &_e;
	font['f'] = &_f;
	font['g'] = &_g;
	font['h'] = &_h;
	font['i'] = &_i;
	font['j'] = &_j;
	font['k'] = &_k;
	font['l'] = &_l;
	font['m'] = &_m;
	font['n'] = &_n;
	font['o'] = &_o;
	font['p'] = &_p;
	font['r'] = &_r;
	font['s'] = &_s;
	font['t'] = &_t;
	font['u'] = &_u;
	font['w'] = &_w;
	font['y'] = &_y;
	font['z'] = &_z;
	font['0'] = &_0;
	font['1'] = &_1;
	font['2'] = &_2;
	font['3'] = &_3;
	font['4'] = &_4;
	font['5'] = &_5;
	font['6'] = &_6;
	font['7'] = &_7;
	font['8'] = &_8;
	font['9'] = &_9;
	font['.'] = &_dot;
	font[','] = &_comma;
	// font['X'] = &_X;
	// font['x'] = &_x;
	// font['_'] = &__;
}

void print(i32 x, i32 y, i32 z, char *str) {
	ui64 str_size = strlen(str);
	i32 px = x;
	i32 py = y;
	struct font *ch;
	char ptr[2];
	char *endptr;
	ui32 color_num;

	for (ui64 i = 0; i < str_size; i++) {
		ch = font[(ui8)str[i]];
		switch ((ui8)str[i]) {
		case '\n':
			py += 10;
			px = x;
			continue;
		case ' ':
			px += 5;
			continue;
		case '^':
			ptr[0] = str[i + 1];
			color_num = (ui32)strtoimax(ptr, &endptr, 10);
			if (color_num > 3 && color_num < 10) {
				pal(21, (i32)color_num, 0);
				i += 1;
			}
			else {
				ptr[1] = str[i + 2];
				color_num = (ui32)strtoimax(ptr, &endptr, 10);
				if (color_num < 32) {
					pal(21, (i32)color_num, 0);
					i += 2;
				}
			}
			continue;
		default:
			if (!ch) {
				continue;
			}
			spr(img, ch->n, px, py + ch->y, 1, 1, 0, z);
			px += ch->x;
		}
	}
	palc();
}
