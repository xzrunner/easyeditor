#include "sl_matrix.h"

#include <string.h>

void 
sl_matrix_identity(struct sl_matrix* mat) {
	memset(mat->e, 0, sizeof(float)*16);
	mat->e[0] = mat->e[5] = mat->e[10] = mat->e[15] = 1;
}

void 
sl_matrix_ortho(struct sl_matrix* mat, float left, float right, float bottom, float top, float near, float far) {
	float a = 2 / (right - left);
	float b = 2 / (top - bottom);
	float c = - (right + left) / (right - left);
	float d = - (top + bottom) / (top - bottom);
	float e = - (far + near) / (far - near);
	float f = -2 / (far - near);
	mat->e[0] = a; mat->e[1] = 0; mat->e[2] = 0; mat->e[3] = 0;
	mat->e[4] = 0; mat->e[5] = b; mat->e[6] = 0; mat->e[7] = 0;
	mat->e[8] = 0; mat->e[9] = 0; mat->e[10] = f; mat->e[11] = 0;
	mat->e[12] = c; mat->e[13] = d; mat->e[14] = e; mat->e[15] = 1;
}

void 
sl_matrix_set_scale(struct sl_matrix* mat, float sx, float sy) {
	mat->e[0] = sx;
	mat->e[5] = sy;
}

void 
sl_matrix_set_translate(struct sl_matrix* mat, float x, float y) {
	mat->e[12] = x;
	mat->e[13] = y;
}