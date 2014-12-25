#ifndef _RT_MATRIX_H_
#define _RT_MATRIX_H_

namespace rt
{

class Matrix {
	public:
		double	m[4][4];

		Matrix();

		Matrix(const Matrix& mat);

		Matrix& operator = (const Matrix& rhs); 	

		Matrix operator * (const Matrix& mat) const;

		Matrix operator / (const double d);

		void SetIdentity(void);	

}; // Matrix

}

#endif // _RT_MATRIX_H_