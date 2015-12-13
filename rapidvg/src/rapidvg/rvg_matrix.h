#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rapid_vector_graphics_matrix_h
#define rapid_vector_graphics_matrix_h

struct rvg_matrix {
	/**
	* | e0 e4 e8  e12 |
	* | e1 e5 e9  e13 |
	* | e2 e6 e10 e14 |
	* | e3 e7 e11 e15 |
	**/
	float e[16];
};

void rvg_matrix_identity(struct rvg_matrix* mat);

void rvg_matrix_set_scale(struct rvg_matrix* mat, float scale);
void rvg_matrix_translate(struct rvg_matrix* mat, float dx, float dy);

void rvg_matrix_ortho(struct rvg_matrix* mat, float left, float right, float bottom, float top, float near, float far);

#endif // rapid_vector_graphics_matrix_h

#ifdef __cplusplus
}
#endif