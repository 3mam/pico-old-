#include "graphic.h"
#include "gl.h"
#include "input.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#include <SDL2/SDL.h>

#define COLOR_PALETTE_SIZE 64
#define COLOR_PALETTE_LEN COLOR_PALETTE_SIZE * 4

static struct draw {
	struct screen {
		i32 x;
		i32 y;
		i32 width;
		i32 height;

		struct clip {
			i32 x;
			i32 y;
			i32 width;
			i32 height;
		} clip;
		i8 palette[COLOR_PALETTE_SIZE];
	} screen;

	struct view {
		i32 x;
		i32 y;
		i32 width;
		i32 height;
	} view;

	struct window {
		i32 width;
		i32 height;
		SDL_Window *handle;
	} window;

	ui32 *canvas;
	ui32 canvas_size;
	ui32 palette[COLOR_PALETTE_SIZE];

	ui8 alpha[COLOR_PALETTE_SIZE];
	i8 *z_buffer;
} self;

static ui8 palette[COLOR_PALETTE_LEN] = {
	255, 0, 64, 0,
	19, 19, 19, 1,
	27, 27, 27, 2,
	39, 39, 39, 3,
	61, 61, 61, 4,
	93, 93, 93, 5,
	133, 133, 133, 6,
	180, 180, 180, 7,
	255, 255, 255, 8,
	199, 207, 221, 9,
	146, 161, 185, 10,
	101, 115, 146, 11,
	66, 76, 110, 12,
	42, 47, 78, 13,
	26, 25, 50, 14,
	14, 7, 27, 15,
	28, 18, 28, 16,
	57, 31, 33, 17,
	93, 44, 40, 18,
	138, 72, 54, 19,
	191, 111, 74, 20,
	230, 156, 105, 21,
	246, 202, 159, 22,
	249, 230, 207, 23,
	237, 171, 80, 24,
	224, 116, 56, 25,
	198, 69, 36, 26,
	142, 37, 29, 27,
	255, 80, 0, 28,
	237, 118, 20, 29,
	255, 162, 20, 30,
	255, 200, 37, 31,
	255, 235, 87, 32,
	211, 252, 126, 33,
	153, 230, 95, 34,
	90, 197, 79, 35,
	51, 152, 75, 36,
	30, 111, 80, 37,
	19, 76, 76, 38,
	12, 46, 68, 39,
	0, 57, 109, 40,
	0, 105, 170, 41,
	0, 152, 220, 42,
	0, 205, 249, 43,
	12, 241, 255, 44,
	148, 253, 255, 45,
	253, 210, 237, 46,
	243, 137, 245, 47,
	219, 63, 253, 48,
	122, 9, 250, 49,
	48, 3, 217, 50,
	12, 2, 147, 51,
	3, 25, 63, 52,
	59, 20, 67, 53,
	98, 36, 97, 54,
	147, 56, 143, 55,
	202, 82, 201, 56,
	200, 80, 134, 57,
	246, 129, 135, 58,
	245, 85, 93, 59,
	234, 50, 60, 60,
	196, 36, 48, 61,
	137, 30, 43, 62,
	87, 28, 39, 63,
};

void get_screen_size(i32 *w, i32 *h) {
	*w = self.screen.width;
	*h = self.screen.height;
}

void resize(void) {
	SDL_GetWindowSize(self.window.handle, &self.window.width,
		&self.window.height);

	f64 targetAspectRatio = (f64)self.screen.width / (f64)self.screen.height;
	self.view.width = self.window.width;
	self.view.height = (i32)((f64)self.view.width / targetAspectRatio);

	if (self.view.height > self.window.height) {
		self.view.height = self.window.height;
		self.view.width = (i32)((f64)self.view.height * targetAspectRatio);
	}

	self.view.x = (self.window.width / 2) - (self.view.width / 2);
	self.view.y = (self.window.height / 2) - (self.view.height / 2);
	glViewport(self.view.x, self.view.y, self.view.width, self.view.height);
	// glViewport(0, 0, self.window.width, self.window.height);
}

void draw_init(i32 width, i32 height) {
	palc();
	self.screen.x = 0;
	self.screen.y = 0;
	self.screen.width = width;
	self.screen.height = height;
	self.screen.clip.x = -1;
	self.screen.clip.y = -1;
	self.screen.clip.width = width;
	self.screen.clip.height = height;

	self.z_buffer = MAKE(i8, (ui32)(width * height));
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Unable to initialize SDL: %s", SDL_GetError());
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		exit(-1);
	}
	self.window.handle = SDL_CreateWindow(
		"Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * 2,
		height * 2, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	self.canvas_size = (ui32)(width * height);
	self.canvas = MAKE(ui32, self.canvas_size);

	SDL_GLContext gl_ctx = SDL_GL_CreateContext(self.window.handle);
	if (gl_ctx == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"OpenGL context could not be created!",
			SDL_GetError(), NULL);
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			"OpenGL context could not be created! SDL Error: %s",
			SDL_GetError());
		exit(-1);
	}
	glInit();
	SDL_GL_SetSwapInterval(1);

	const GLchar *verTex = "attribute vec2 a_position;"
		"attribute vec2 a_texCoord;"
		"varying vec2 v_texCoord;"
		"void main() {"
		"gl_Position = vec4(a_position, 0, 1);"
		"v_texCoord = a_texCoord;"
		"}";
	const GLchar *fragTex =
#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
		"precision mediump float;\n"
#endif
		"uniform sampler2D u_image;"
		"varying vec2 v_texCoord;"
		"void main() {"
		"gl_FragColor = texture2D(u_image, v_texCoord);"
		"}";

	GLuint program = glCreateShaders(verTex, fragTex);
	GLuint position_location = glGetAttribLocation(program, "a_position");
	GLuint tex_coord_location = glGetAttribLocation(program, "a_texCoord");
	glUseProgram(program);

	GLuint quad_id;
	glGenBuffers(1, &quad_id);
	glBindBuffer(GL_ARRAY_BUFFER, quad_id);
	GLfloat quad[] = { -1.0, 1.0,  1.0, 1.0, -1.0, -1.0,
										-1.0, -1.0, 1.0, 1.0, 1.0,  -1.0 };
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint quad_cor_id;
	glGenBuffers(1, &quad_cor_id);
	glBindBuffer(GL_ARRAY_BUFFER, quad_cor_id);
	GLfloat qaud_cor[] = { 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
												0.0, 1.0, 1.0, 0.0, 1.0, 1.0 };
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, qaud_cor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(tex_coord_location);
	glVertexAttribPointer(tex_coord_location, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	resize();
}

void camera(i32 x, i32 y) {
	self.screen.x = x;
	self.screen.y = y;
}

void draw(void) {
	ui8 c = 0;
	for (ui32 i = self.canvas_size; i--;) {
		c = (ui8)(self.canvas[i] >> 24);
		if (self.screen.palette[c] != -1) {
			self.canvas[i] = palette[self.screen.palette[c]];
		}
	}

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				resize();
				break;
			case SDL_WINDOWEVENT_CLOSE:
				SDL_Quit();
				exit(EXIT_SUCCESS);
				break;
			}
			break;
		case SDL_KEYDOWN:
			keys[event.key.keysym.scancode] = 1;
			break;
		case SDL_KEYUP:
			keys[event.key.keysym.scancode] = 0;
			break;
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);
	// glClearColor(1, 1, 1, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self.screen.width, self.screen.height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, self.canvas);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	SDL_GL_SwapWindow(self.window.handle);
}

void pset(i32 x, i32 y, i32 c, i32 a, i32 z) {
	x -= self.screen.x;
	y -= self.screen.y;
	if (self.alpha[c] == 255 && x < self.screen.width && x > -1 &&
		y < self.screen.height && y > -1 && x < self.screen.clip.width &&
		x > self.screen.clip.x && y < self.screen.clip.height &&
		y > self.screen.clip.y) {

		i32 xy = ((y * self.screen.width) + x);
		if (self.z_buffer[xy] > z) {
			return;
		}
		self.z_buffer[xy] = (i8)(z);
		if (a < 255) {
			ui32 rb = self.palette[c] & 0xff00ff;
			ui32 g = self.palette[c] & 0x00ff00;
			rb += ((self.canvas[xy] & 0xff00ff) - rb) * (255 - (ui32)(a)) >> 8;
			g += ((self.canvas[xy] & 0x00ff00) - g) * (255 - (ui32)(a)) >> 8;
			self.canvas[xy] = (rb & 0xff00ff) | (g & 0xff00);
		}
		else {
			self.canvas[xy] = self.palette[c];
		}
	}
}

void line(i32 x0, i32 y0, i32 x1, i32 y1, i32 col, i32 a, i32 z) {
	i32 dx = abs(x1 - x0);
	i32 dy = -abs(y1 - y0);
	i32 sx = x0 < x1 ? 1 : -1;
	i32 sy = y0 < y1 ? 1 : -1;
	i32 err = dx + dy;
	i32 e2 = 0;

	for (;;) {
		pset(x0, y0, col, a, z);

		if (x0 == x1 && y0 == y1) {
			break;
		}

		e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}

		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void rect(i32 x0, i32 y0, i32 x1, i32 y1, i32 col, i32 a, i32 z) {
	line(x0, y0, x1, y0, col, a, z);
	line(x1, y0, x1, y1, col, a, z);
	line(x1, y1, x0, y1, col, a, z);
	line(x0, y1, x0, y0, col, a, z);
}

void rect_fill(i32 x0, i32 y0, i32 x1, i32 y1, i32 col, i32 a, i32 z) {
	for (i32 y = y0; y < y1 + 1; y++) {
		for (i32 x = x0; x < x1 + 1; x++) {
			pset(x, y, col, a, z);
		}
	}
}

void circ(i32 x0, i32 y0, i32 r, i32 col, i32 a, i32 z) {
	i32 x = -r;
	i32 y = 0;
	i32 err = 2 - 2 * r; // r * 2 - 2

	for (; x < -1;) {
		r = err;
		if (r > x) {
			x++;
			err += x * 2 + 1;
		}
		if (r <= y) {
			y++;
			err += y * 2 + 1;
		}

		pset(x0 + x, y0 - y, col, a, z);
		pset(x0 + x, y0 + y - 1, col, a, z);
		pset(x0 - x - 1, y0 - y, col, a, z);
		pset(x0 - x - 1, y0 + y - 1, col, a, z);
	}
}

void circ_fill(i32 x0, i32 y0, i32 r, i32 col, i32 a, i32 z) {
	i32 x = -r;
	i32 y = 0;
	i32 err = 2 - 2 * r; // r * 2 - 2
	i32 y_repit = 0;

	for (; x < 0;) {
		r = err;
		if (r > x) {
			x++;
			err += x * 2 + 1;
		}
		if (r <= y) {
			y++;
			err += y * 2 + 1;
		}

		if (y_repit == y) {
			continue;
		}
		y_repit = y;

		for (i32 i = x; i < 0; i++) {
			pset(x0 + x - i - 1, y0 - y, col, a, z);
			pset(x0 + x - i - 1, y0 + y - 1, col, a, z);
			pset(x0 - x + i, y0 - y, col, a, z);
			pset(x0 - x + i, y0 + y - 1, col, a, z);
		}
	}
}

void tri(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, i32 col, i32 a,
	i32 z) {
	line(x0, y0, x1, y1, col, a, z);
	line(x1, y1, x2, y2, col, a, z);
	line(x0, y0, x2, y2, col, a, z);
}

static void sort2dvectors(f64 list[3][2]) {
	for (i32 i = 0; i < 3; i++) {
		for (i32 j = 0; j < 3; j++) {
			if (i != j) {
				f64 x1 = list[i][0];
				f64 y1 = list[i][1];
				f64 x2 = list[j][0];
				f64 y2 = list[j][1];
				if (y2 > y1) {
					f64 tmp[2];
					tmp[0] = list[i][0];
					tmp[1] = list[i][1];
					list[i][0] = list[j][0];
					list[i][1] = list[j][1];
					list[j][0] = tmp[0];
					list[j][1] = tmp[1];
				}
				else if (y2 == y1) {
					if (x2 > x1) {
						f64 tmp[2];
						tmp[0] = list[i][0];
						tmp[1] = list[i][1];
						list[i][0] = list[j][0];
						list[i][1] = list[j][1];
						list[j][0] = tmp[0];
						list[j][1] = tmp[1];
					}
				}
			}
		}
	}
}

void tri_fill(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, i32 col, i32 a,
	i32 z) {
	f64 list[3][2];
	list[0][0] = (f64)(x0);
	list[0][1] = (f64)(y0);
	list[1][0] = (f64)(x1);
	list[1][1] = (f64)(y1);
	list[2][0] = (f64)(x2);
	list[2][1] = (f64)(y2);

	sort2dvectors(list);

	f64 xs = list[0][0];
	f64 xe = list[0][0];
	f64 vx1 = (list[1][0] - list[0][0]) / (list[1][1] - list[0][1]);
	f64 vx2 = (list[2][0] - list[1][0]) / (list[2][1] - list[1][1]);
	f64 vx3 = (list[2][0] - list[0][0]) / (list[2][1] - list[0][1]);

	if (floor(list[1][1] - list[0][1]) == 0) {
		vx2 = vx3;
		xe = list[1][0];
		vx3 = (list[2][0] - list[1][0]) / (list[2][1] - list[1][1]);
	}

	for (f64 y = list[0][1]; y < list[2][1]; y++) {
		f64 l = sqrt((xs - xe) * (xs - xe));

		for (f64 x = 0; x < -floor(-(l)); x++) {
			pset((i32)(x + floor(fmin(xs, xe))), (i32)(y), col, a, z);
		}

		if (y < list[1][1]) {
			xs += vx1;
		}
		else if (y >= list[1][1]) {
			xs += vx2;
		}
		xe += vx3;
	}
}

sprite *spr_load(char *name) {
	sprite *s = NEW(sprite);
	ui8 *img = stbi_load(name, &s->w, &s->h, &s->n, 4);
	s->canvas = MAKE(ui8, s->w*s->h);
	for (i32 i = s->w*s->h; i--;) {
		for (i32 j = COLOR_PALETTE_LEN; j -= 4;) {
			if (img[i * 4] == palette[j] &&
				img[i * 4 + 1] == palette[j + 1] &&
				img[i * 4 + 2] == palette[j + 2]) {
				s->canvas[i] = j / 4;
			}
		}
	}

	free(img);

	s->cell = MAKE(sprite_sector, (s->w / 10) * (s->h / 10));
	i32 n = (s->w / 10) * (s->h / 10);
	n--;
	for (i32 y = s->h / 10; y--;) {
		for (i32 x = s->w / 10; x--;) {
			s->cell[n].x = (x * 10);
			s->cell[n].y = (y * 10);
			n--;
		}
	}
	return s;
}

void spr(sprite *s, i32 n, i32 x, i32 y, i32 w, i32 h, i32 flip, i32 z) {
	x -= self.screen.x;
	y -= self.screen.y;
	ui32 *pal = self.palette;
	i32 c = 0;
	i32 ww = 0;
	i32 hh = 0;
	i32 x0 = 0;
	i32 y0 = 0;

	if (w == 0 && h == 0 && n == 0) {
		w = s->w / 10;
		h = s->h / 10;
	}

	ww = s->cell[n].x + (w * 10);
	hh = s->cell[n].y + (h * 10);

	for (i32 yy = s->cell[n].y; yy < hh; yy++) {
		y0 = (yy - hh) + y + (10 * h);
		if (y0 < self.screen.height && y0 > -1 && y0 < self.screen.clip.height &&
			y0 > self.screen.clip.y) {
			for (i32 xx = s->cell[n].x; xx < ww; xx++) {
				x0 = (xx - ww) + x + (10 * w);
				if (x0 < self.screen.width && x0 > -1 && x0 < self.screen.clip.width &&
					x0 > self.screen.clip.x) {
					switch (flip) {
					case 3:
						c = s->canvas[((-yy + hh) * s->w) + (-xx + ww)];
						break;
					case 2:
						c = s->canvas[((-yy + hh) * s->w) + xx];
						break;
					case 1:
						c = s->canvas[(yy * s->w) + (-xx + ww)];
						break;
					case 0:
						c = s->canvas[((yy * s->w) + xx)];
					}
					if (self.z_buffer[y0 * self.screen.width + x0] > z) {
						continue;
					}
					if (self.alpha[c] == 255) {
						self.z_buffer[y0 * self.screen.width + x0] = (i8)(z);
						self.canvas[y0 * self.screen.width + x0] = pal[c];
					}
				}
			}
		}
	}
}

/*
void spr(sprite *s, i32 frame, i32 dx, i32 dy, i32 dw, i32 dh, i32 flip, i32 a,
				 i32 z) {
	dx -= self.screen.x;
	dy -= self.screen.y;
	ui32 *pal = self.palette;
	i32 c = 0;
	i32 x_ratio = (s->w << 16) / dw;
	i32 y_ratio = (s->h << 16) / dh;
	i32 x2 = 0;
	i32 y2 = 0;
	i32 x0 = 0;
	i32 y0 = 0;
	i32 yws = 0;
	i32 yhs = 0;
	i32 ywc = 0;
	i32 ywcx = 0;
	ui32 rb = 0;
	ui32 g = 0;

	for (i32 y = dh; y--;) {
		y0 = y + dy;
		y2 = (y * y_ratio) >> 16;
		yws = y2 * s->w;
		ywc = y0 * self.screen.width;
		yhs = (-y2 + s->h - 1) * s->w;

		if (y0 < self.screen.height && y0 > -1 && y0 < self.screen.clip.height &&
				y0 > self.screen.clip.y) {

			for (i32 x = dw; x--;) {
				x0 = x + dx;
				if (x0 < self.screen.width && x0 > -1 && x0 < self.screen.clip.width &&
						x0 > self.screen.clip.x) {
					x2 = (x * x_ratio) >> 16;
					switch (flip) {
					case 3:
						c = s->canvas[((yhs + (-x2 + s->w - 1)) * s->frames) + frame];
						break;
					case 2:
						c = s->canvas[((yhs + x2) * s->frames) + frame];
						break;
					case 1:
						c = s->canvas[((yws + (-x2 + s->w - 1)) * s->frames) + frame];
						break;
					case 0:
						c = s->canvas[((yws + x2) * s->frames) + frame];
					}
					ywcx = ywc + x0;
					if (self.z_buffer[ywcx] > z) {
						continue;
					}
					if (self.alpha[c] == 255) {
						self.z_buffer[ywcx] = (i8)(z);
						if (a < 255) {
							rb = pal[c] & 0xff00ff;
							g = pal[c] & 0x00ff00;
							rb += ((self.canvas[ywcx] & 0xff00ff) - rb) * (255 - (ui32)(a)) >>
										8;
							g +=
									((self.canvas[ywcx] & 0x00ff00) - g) * (255 - (ui32)(a)) >> 8;
							self.canvas[ywcx] = (rb & 0xff00ff) | (g & 0xff00);
						} else {
							//*(self.canvas + ywcx) = pal[c];
							self.canvas[ywcx] = pal[c];
						}
					}
				}
			}
		}
	}
}

void spr2(sprite *s, i32 dx, i32 dy) {
	dx -= self.screen.x;
	dy -= self.screen.y;
	ui32 *pal = self.palette;
	i32 c = 0;
	i32 yws = 0;
	i32 ywc = 0;
	i32 x0 = 0;
	i32 y0 = 0;

	for (i32 y = s->h; y--;) {
		yws = y * s->w;
		y0 = y + dy;
		ywc = y0 * self.screen.width;
		if (y0 < self.screen.height && y0 > -1 && y0 < self.screen.clip.height &&
				y0 > self.screen.clip.y) {
			for (i32 x = s->w; x--;) {
				x0 = x + dx;
				if (x0 < self.screen.width && x0 > -1 && x0 < self.screen.clip.width &&
						x0 > self.screen.clip.x) {
					c = s->canvas[yws + x];
					self.canvas[ywc + x0] = pal[c];
					//*(self.canvas + ywc + x0) = pal[c];
				}
			}
		}
	}
}
*/
void cls(i32 col) {
	for (ui32 n = self.canvas_size; n--;) {
		self.canvas[n] = self.palette[col];
		self.z_buffer[n] = 0;
	}
}

void pal(i32 c0, i32 c1, i32 t) {
	if (t) {
		self.screen.palette[c0] = (i8)c1;
	}
	else {
		ui32 *p = (ui32 *)palette;
		self.palette[c0] = p[c1];
	}
}

void palc(void) {
	memset(self.screen.palette, -1, COLOR_PALETTE_SIZE);
	memset(self.alpha, 255, COLOR_PALETTE_SIZE);
	self.alpha[0] = 0;
	memcpy(self.palette, palette, COLOR_PALETTE_LEN);
}

void palt(i32 c, i32 t) {
	if (t) {
		self.alpha[c] = 1;
	}
	else {
		self.alpha[c] = 0;
	}
}

void clip(i32 x, i32 y, i32 w, i32 h) {
	self.screen.clip.x = x - 1;
	self.screen.clip.y = y - 1;
	self.screen.clip.width = w;
	self.screen.clip.height = h;
}

i32 pget(i32 x, i32 y) {
	if (x < self.screen.width && x > -1 && y < self.screen.height && y > -1) {
		i32 xy = ((y * self.screen.width) + x);
		return self.canvas[xy] >> 24;
	}
	return -1;
}
