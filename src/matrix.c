#include "matrix.h"
#include "string.h"

static void multiplyMatrices4x4(f32 dst[16], f32 src[16]) {
	f32 tmp[16] = {
			src[0] * dst[0] + src[1] * dst[4] + src[2] * dst[8] + src[3] * dst[12],
			src[0] * dst[1] + src[1] * dst[5] + src[2] * dst[9] + src[3] * dst[13],
			src[0] * dst[2] + src[1] * dst[6] + src[2] * dst[10] + src[3] * dst[14],
			src[0] * dst[3] + src[1] * dst[7] + src[2] * dst[11] + src[3] * dst[15],
			src[4] * dst[0] + src[5] * dst[4] + src[6] * dst[8] + src[7] * dst[12],
			src[4] * dst[1] + src[5] * dst[5] + src[6] * dst[9] + src[7] * dst[13],
			src[4] * dst[2] + src[5] * dst[6] + src[6] * dst[10] + src[7] * dst[14],
			src[4] * dst[3] + src[5] * dst[7] + src[6] * dst[11] + src[7] * dst[15],
			src[8] * dst[0] + src[9] * dst[4] + src[10] * dst[8] + src[11] * dst[12],
			src[8] * dst[1] + src[9] * dst[5] + src[10] * dst[9] + src[11] * dst[13],
			src[8] * dst[2] + src[9] * dst[6] + src[10] * dst[10] + src[11] * dst[14],
			src[8] * dst[3] + src[9] * dst[7] + src[10] * dst[11] + src[11] * dst[15],
			src[12] * dst[0] + src[13] * dst[4] + src[14] * dst[8] +
					src[15] * dst[12],
			src[12] * dst[1] + src[13] * dst[5] + src[14] * dst[9] +
					src[15] * dst[13],
			src[12] * dst[2] + src[13] * dst[6] + src[14] * dst[10] +
					src[15] * dst[14],
			src[12] * dst[3] + src[13] * dst[7] + src[14] * dst[11] +
					src[15] * dst[15],
	};
	memcpy(dst, tmp, sizeof(tmp));
}

void scale(f32 m[16], f32 x, f32 y, f32 z) {
	f32 tmp[16] = {
			x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1,
	};
	multiplyMatrices4x4(m, tmp);
}

void ortho(f32 m[16], f32 left, f32 right, f32 bottom, f32 top, f32 nearval,
					 f32 farval) {
	f32 tmp[16];

	tmp[0] = 2.0f / (right - left);
	tmp[1] = 0.0;
	tmp[2] = 0.0;
	tmp[3] = -(right + left) / (right - left);

	tmp[4] = 0.0;
	tmp[5] = 2.0f / (top - bottom);
	tmp[6] = 0.0;
	tmp[7] = -(top + bottom) / (top - bottom);

	tmp[8] = 0.0;
	tmp[9] = 0.0;
	tmp[10] = -2.0f / (farval - nearval);
	tmp[11] = -(farval + nearval) / (farval - nearval);

	tmp[12] = 0.0;
	tmp[13] = 0.0;
	tmp[14] = 0.0;
	tmp[15] = 1.0;
	multiplyMatrices4x4(m, tmp);
}

void reset(f32 m[16]) {
	m[0] = 1.0;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 0.0;
	m[4] = 0.0;
	m[5] = 1.0;
	m[6] = 0.0;
	m[7] = 0.0;
	m[8] = 0.0;
	m[9] = 0.0;
	m[10] = 1.0;
	m[11] = 0.0;
	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;
}
