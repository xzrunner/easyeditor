#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_matrix_h
#define shader_lab_matrix_h

struct sl_matrix {
	/**
	* | e0 e4 e8  e12 |
	* | e1 e5 e9  e13 |
	* | e2 e6 e10 e14 |
	* | e3 e7 e11 e15 |
	**/
	float e[16];
};

void sl_matrix_identity(struct sl_matrix* mat);

void sl_matrix_set_scale(struct sl_matrix* mat, float sx, float sy);
void sl_matrix_set_translate(struct sl_matrix* mat, float x, float y);

void sl_matrix_ortho(struct sl_matrix* mat, float left, float right, float bottom, float top, float near, float far);

#endif // shader_lab_matrix_h

#ifdef __cplusplus
}
#endif