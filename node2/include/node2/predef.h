#pragma once

#ifdef N2_MATRIX_FIX
	#define N2_MAT sm::MatrixFix
	#define N2_MAT_HEADER <SM_MatrixFix.h>
#else
	#define N2_MAT sm::Matrix2D
	#define N2_MAT_HEADER <SM_Matrix2D.h>
#endif // N2_MATRIX_FIX
