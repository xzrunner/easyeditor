#pragma once
#include "Vector.h"

template <typename T>
struct Matrix2 {
	Matrix2()
	{
		c[0][0] = 1; c[0][1] = 0;
		c[1][0] = 0; c[1][1] = 1;
	}
	Matrix2(const T* m)
	{
		c[0][0] = m[0]; c[0][1] = m[1];
		c[1][0] = m[2]; c[1][1] = m[3];
	}
	const T* Pointer() const
	{
		return &c[0][0];
	}

	union {
		float c[2][2];	// Column major order for OpenGL: c[column][row]
		float x[4];
	};
};

template <typename T>
struct Matrix3 {
	Matrix3()
	{
		c[0][0] = 1; c[0][1] = 0; c[0][2] = 0;
		c[1][0] = 0; c[1][1] = 1; c[1][2] = 0;
		c[2][0] = 0; c[2][1] = 0; c[2][2] = 1;
	}
	Matrix3(const T* m)
	{
		c[0][0] = m[0]; c[0][1] = m[1]; c[0][2] = m[2];
		c[1][0] = m[3]; c[1][1] = m[4]; c[1][2] = m[5];
		c[2][0] = m[6]; c[2][1] = m[7]; c[2][2] = m[8];
	}
	Matrix3 Transposed() const
	{
		Matrix3 m;
		m.c[0][0] = c[0][0]; m.c[0][1] = c[1][0]; m.c[0][2] = c[2][0];
		m.c[1][0] = c[0][1]; m.c[1][1] = c[1][1]; m.c[1][2] = c[2][1];
		m.c[2][0] = c[0][2]; m.c[2][1] = c[1][2]; m.c[2][2] = c[2][2];
		return m;
	}
	const T* Pointer() const
	{
		return &c[0][0];
	}

	union {
		float c[3][3];	// Column major order for OpenGL: c[column][row]
		float x[9];
	};
};

template <typename T>
struct Matrix4 {
	Matrix4()
	{
		c[0][0] = 1; c[0][1] = 0; c[0][2] = 0; c[0][3] = 0;
		c[1][0] = 0; c[1][1] = 1; c[1][2] = 0; c[1][3] = 0;
		c[2][0] = 0; c[2][1] = 0; c[2][2] = 1; c[2][3] = 0;
		c[3][0] = 0; c[3][1] = 0; c[3][2] = 0; c[3][3] = 1;
	}
	Matrix4(const Matrix3<T>& m)
	{
		c[0][0] = m.c[0][0]; c[0][1] = m.c[0][1]; c[0][2] = m.c[0][2]; c[0][3] = 0;
		c[1][0] = m.c[1][0]; c[1][1] = m.c[1][1]; c[1][2] = m.c[1][2]; c[1][3] = 0;
		c[2][0] = m.c[2][0]; c[2][1] = m.c[2][1]; c[2][2] = m.c[2][2]; c[2][3] = 0;
		c[3][0] = 0; c[3][1] = 0; c[3][2] = 0; c[3][3] = 1;
	}
	Matrix4(const T* m)
	{
		c[0][0] = m[0];  c[0][1] = m[1];  c[0][2] = m[2];  c[0][3] = m[3];
		c[1][0] = m[4];  c[1][1] = m[5];  c[1][2] = m[6];  c[1][3] = m[7];
		c[2][0] = m[8];  c[2][1] = m[9];  c[2][2] = m[10]; c[2][3] = m[11];
		c[3][0] = m[12]; c[3][1] = m[13]; c[3][2] = m[14]; c[3][3] = m[15];
	}
	void Assign(const T* m)
	{
		c[0][0] = m[0];  c[0][1] = m[1];  c[0][2] = m[2];  c[0][3] = m[3];
		c[1][0] = m[4];  c[1][1] = m[5];  c[1][2] = m[6];  c[1][3] = m[7];
		c[2][0] = m[8];  c[2][1] = m[9];  c[2][2] = m[10]; c[2][3] = m[11];
		c[3][0] = m[12]; c[3][1] = m[13]; c[3][2] = m[14]; c[3][3] = m[15];
	}
	Matrix4 operator * (const Matrix4& b) const
	{
		Matrix4 m;
		m.c[0][0] = c[0][0] * b.c[0][0] + c[0][1] * b.c[1][0] + c[0][2] * b.c[2][0] + c[0][3] * b.c[3][0];
		m.c[0][1] = c[0][0] * b.c[0][1] + c[0][1] * b.c[1][1] + c[0][2] * b.c[2][1] + c[0][3] * b.c[3][1];
		m.c[0][2] = c[0][0] * b.c[0][2] + c[0][1] * b.c[1][2] + c[0][2] * b.c[2][2] + c[0][3] * b.c[3][2];
		m.c[0][3] = c[0][0] * b.c[0][3] + c[0][1] * b.c[1][3] + c[0][2] * b.c[2][3] + c[0][3] * b.c[3][3];
		m.c[1][0] = c[1][0] * b.c[0][0] + c[1][1] * b.c[1][0] + c[1][2] * b.c[2][0] + c[1][3] * b.c[3][0];
		m.c[1][1] = c[1][0] * b.c[0][1] + c[1][1] * b.c[1][1] + c[1][2] * b.c[2][1] + c[1][3] * b.c[3][1];
		m.c[1][2] = c[1][0] * b.c[0][2] + c[1][1] * b.c[1][2] + c[1][2] * b.c[2][2] + c[1][3] * b.c[3][2];
		m.c[1][3] = c[1][0] * b.c[0][3] + c[1][1] * b.c[1][3] + c[1][2] * b.c[2][3] + c[1][3] * b.c[3][3];
		m.c[2][0] = c[2][0] * b.c[0][0] + c[2][1] * b.c[1][0] + c[2][2] * b.c[2][0] + c[2][3] * b.c[3][0];
		m.c[2][1] = c[2][0] * b.c[0][1] + c[2][1] * b.c[1][1] + c[2][2] * b.c[2][1] + c[2][3] * b.c[3][1];
		m.c[2][2] = c[2][0] * b.c[0][2] + c[2][1] * b.c[1][2] + c[2][2] * b.c[2][2] + c[2][3] * b.c[3][2];
		m.c[2][3] = c[2][0] * b.c[0][3] + c[2][1] * b.c[1][3] + c[2][2] * b.c[2][3] + c[2][3] * b.c[3][3];
		m.c[3][0] = c[3][0] * b.c[0][0] + c[3][1] * b.c[1][0] + c[3][2] * b.c[2][0] + c[3][3] * b.c[3][0];
		m.c[3][1] = c[3][0] * b.c[0][1] + c[3][1] * b.c[1][1] + c[3][2] * b.c[2][1] + c[3][3] * b.c[3][1];
		m.c[3][2] = c[3][0] * b.c[0][2] + c[3][1] * b.c[1][2] + c[3][2] * b.c[2][2] + c[3][3] * b.c[3][2];
		m.c[3][3] = c[3][0] * b.c[0][3] + c[3][1] * b.c[1][3] + c[3][2] * b.c[2][3] + c[3][3] * b.c[3][3];
		return m;
	}
	Matrix4& operator *= (const Matrix4& b)
	{
		Matrix4 m = *this * b;
		return (*this = m);
	}
	Vector3<T> operator * (const Vector3<T>& b) const
	{
		Vector3<T> v;
		v.x = b.x * c[0][0] + b.y * c[1][0] + b.z * c[2][0] + c[3][0];
		v.y = b.x * c[0][1] + b.y * c[1][1] + b.z * c[2][1] + c[3][1];
		v.z = b.x * c[0][2] + b.y * c[1][2] + b.z * c[2][2] + c[3][2];
		return v;
	}
	Matrix4 Transposed() const
	{
		Matrix4 m;
		m.c[0][0] = c[0][0]; m.c[0][1] = c[1][0]; m.c[0][2] = c[2][0]; m.c[0][3] = c[3][0];
		m.c[1][0] = c[0][1]; m.c[1][1] = c[1][1]; m.c[1][2] = c[2][1]; m.c[1][3] = c[3][1];
		m.c[2][0] = c[0][2]; m.c[2][1] = c[1][2]; m.c[2][2] = c[2][2]; m.c[2][3] = c[3][2];
		m.c[3][0] = c[0][3]; m.c[3][1] = c[1][3]; m.c[3][2] = c[2][3]; m.c[3][3] = c[3][3];
		return m;
	}
	Matrix3<T> ToMat3() const
	{
		Matrix3<T> m;
		m.c[0][0] = c[0][0]; m.c[1][0] = c[1][0]; m.c[2][0] = c[2][0];
		m.c[0][1] = c[0][1]; m.c[1][1] = c[1][1]; m.c[2][1] = c[2][1];
		m.c[0][2] = c[0][2]; m.c[1][2] = c[1][2]; m.c[2][2] = c[2][2];
		return m;
	}
	const T* Pointer() const
	{
		return &c[0][0];
	}
	static Matrix4<T> Identity()
	{
		return Matrix4();
	}
	static Matrix4<T> Translate(T x, T y, T z)
	{
		Matrix4 m;
		m.c[0][0] = 1; m.c[0][1] = 0; m.c[0][2] = 0; m.c[0][3] = 0;
		m.c[1][0] = 0; m.c[1][1] = 1; m.c[1][2] = 0; m.c[1][3] = 0;
		m.c[2][0] = 0; m.c[2][1] = 0; m.c[2][2] = 1; m.c[2][3] = 0;
		m.c[3][0] = x; m.c[3][1] = y; m.c[3][2] = z; m.c[3][3] = 1;
		return m;
	}
	static Matrix4<T> Scale(T s)
	{
		Matrix4 m;
		m.c[0][0] = s; m.c[0][1] = 0; m.c[0][2] = 0; m.c[0][3] = 0;
		m.c[1][0] = 0; m.c[1][1] = s; m.c[1][2] = 0; m.c[1][3] = 0;
		m.c[2][0] = 0; m.c[2][1] = 0; m.c[2][2] = s; m.c[2][3] = 0;
		m.c[3][0] = 0; m.c[3][1] = 0; m.c[3][2] = 0; m.c[3][3] = 1;
		return m;
	}

	static Matrix4<T> RotateX(T degrees)
	{
		T radians = degrees * 3.14159f / 180.0f;
		T s = sin(radians);
		T c = cos(radians);

		Matrix4 m;
		m.c[0][0] = 1; m.c[0][1] = 0; m.c[0][2] = 0; m.c[0][3] = 0;
		m.c[1][0] = 0; m.c[1][1] = c; m.c[1][2] = s; m.c[1][3] = 0;
		m.c[2][0] = 0; m.c[2][1] =-s; m.c[2][2] = c; m.c[2][3] = 0;
		m.c[3][0] = 0; m.c[3][1] = 0; m.c[3][2] = 0; m.c[3][3] = 1;
		return m;
	}
	static Matrix4<T> RotateY(T degrees)
	{
		T radians = degrees * 3.14159f / 180.0f;
		T s = sin(radians);
		T c = cos(radians);

		Matrix4 m;
		m.c[0][0] = c; m.c[0][1] = 0; m.c[0][2] =-s; m.c[0][3] = 0;
		m.c[1][0] = 0; m.c[1][1] = 1; m.c[1][2] = 0; m.c[1][3] = 0;
		m.c[2][0] = s; m.c[2][1] = 0; m.c[2][2] = c; m.c[2][3] = 0;
		m.c[3][0] = 0; m.c[3][1] = 0; m.c[3][2] = 0; m.c[3][3] = 1;
		return m;
	}
	static Matrix4<T> RotateZ(T degrees)
	{
		T radians = degrees * 3.14159f / 180.0f;
		T s = sin(radians);
		T c = cos(radians);

		Matrix4 m;
		m.c[0][0] = c; m.c[0][1] = s; m.c[0][2] = 0; m.c[0][3] = 0;
		m.c[1][0] =-s; m.c[1][1] = c; m.c[1][2] = 0; m.c[1][3] = 0;
		m.c[2][0] = 0; m.c[2][1] = 0; m.c[2][2] = 1; m.c[2][3] = 0;
		m.c[3][0] = 0; m.c[3][1] = 0; m.c[3][2] = 0; m.c[3][3] = 1;
		return m;
	}

	Matrix4<T> Invert()
	{
		double inv[16], det;
		int i;

		inv[0] = x[5]  * x[10] * x[15] - 
			x[5]  * x[11] * x[14] - 
			x[9]  * x[6]  * x[15] + 
			x[9]  * x[7]  * x[14] +
			x[13] * x[6]  * x[11] - 
			x[13] * x[7]  * x[10];

		inv[4] = -x[4]  * x[10] * x[15] + 
			x[4]  * x[11] * x[14] + 
			x[8]  * x[6]  * x[15] - 
			x[8]  * x[7]  * x[14] - 
			x[12] * x[6]  * x[11] + 
			x[12] * x[7]  * x[10];

		inv[8] = x[4]  * x[9] * x[15] - 
			x[4]  * x[11] * x[13] - 
			x[8]  * x[5] * x[15] + 
			x[8]  * x[7] * x[13] + 
			x[12] * x[5] * x[11] - 
			x[12] * x[7] * x[9];

		inv[12] = -x[4]  * x[9] * x[14] + 
			x[4]  * x[10] * x[13] +
			x[8]  * x[5] * x[14] - 
			x[8]  * x[6] * x[13] - 
			x[12] * x[5] * x[10] + 
			x[12] * x[6] * x[9];

		inv[1] = -x[1]  * x[10] * x[15] + 
			x[1]  * x[11] * x[14] + 
			x[9]  * x[2] * x[15] - 
			x[9]  * x[3] * x[14] - 
			x[13] * x[2] * x[11] + 
			x[13] * x[3] * x[10];

		inv[5] = x[0]  * x[10] * x[15] - 
			x[0]  * x[11] * x[14] - 
			x[8]  * x[2] * x[15] + 
			x[8]  * x[3] * x[14] + 
			x[12] * x[2] * x[11] - 
			x[12] * x[3] * x[10];

		inv[9] = -x[0]  * x[9] * x[15] + 
			x[0]  * x[11] * x[13] + 
			x[8]  * x[1] * x[15] - 
			x[8]  * x[3] * x[13] - 
			x[12] * x[1] * x[11] + 
			x[12] * x[3] * x[9];

		inv[13] = x[0]  * x[9] * x[14] - 
			x[0]  * x[10] * x[13] - 
			x[8]  * x[1] * x[14] + 
			x[8]  * x[2] * x[13] + 
			x[12] * x[1] * x[10] - 
			x[12] * x[2] * x[9];

		inv[2] = x[1]  * x[6] * x[15] - 
			x[1]  * x[7] * x[14] - 
			x[5]  * x[2] * x[15] + 
			x[5]  * x[3] * x[14] + 
			x[13] * x[2] * x[7] - 
			x[13] * x[3] * x[6];

		inv[6] = -x[0]  * x[6] * x[15] + 
			x[0]  * x[7] * x[14] + 
			x[4]  * x[2] * x[15] - 
			x[4]  * x[3] * x[14] - 
			x[12] * x[2] * x[7] + 
			x[12] * x[3] * x[6];

		inv[10] = x[0]  * x[5] * x[15] - 
			x[0]  * x[7] * x[13] - 
			x[4]  * x[1] * x[15] + 
			x[4]  * x[3] * x[13] + 
			x[12] * x[1] * x[7] - 
			x[12] * x[3] * x[5];

		inv[14] = -x[0]  * x[5] * x[14] + 
			x[0]  * x[6] * x[13] + 
			x[4]  * x[1] * x[14] - 
			x[4]  * x[2] * x[13] - 
			x[12] * x[1] * x[6] + 
			x[12] * x[2] * x[5];

		inv[3] = -x[1] * x[6] * x[11] + 
			x[1] * x[7] * x[10] + 
			x[5] * x[2] * x[11] - 
			x[5] * x[3] * x[10] - 
			x[9] * x[2] * x[7] + 
			x[9] * x[3] * x[6];

		inv[7] = x[0] * x[6] * x[11] - 
			x[0] * x[7] * x[10] - 
			x[4] * x[2] * x[11] + 
			x[4] * x[3] * x[10] + 
			x[8] * x[2] * x[7] - 
			x[8] * x[3] * x[6];

		inv[11] = -x[0] * x[5] * x[11] + 
			x[0] * x[7] * x[9] + 
			x[4] * x[1] * x[11] - 
			x[4] * x[3] * x[9] - 
			x[8] * x[1] * x[7] + 
			x[8] * x[3] * x[5];

		inv[15] = x[0] * x[5] * x[10] - 
			x[0] * x[6] * x[9] - 
			x[4] * x[1] * x[10] + 
			x[4] * x[2] * x[9] + 
			x[8] * x[1] * x[6] - 
			x[8] * x[2] * x[5];

		det = x[0] * inv[0] + x[1] * inv[4] + x[2] * inv[8] + x[3] * inv[12];

		if (det == 0) {
//			assert(0);
			return Matrix4::Identity();
		}

		det = 1.0 / det;

		Matrix4 m;
		for (i = 0; i < 16; i++)
			m.x[i] = inv[i] * det;
		return m;
	}

	static Matrix4<T> Perspective(T left, T right, T bottom, T top, T _near, T _far)
	{
		T a = 2 * _near / (right - left);
		T b = 2 * _near / (top - bottom);
		T c = (right + left) / (right - left);
		T d = (top + bottom) / (top - bottom);
		T e = -2 * _near;
		Matrix4 m;
		m.c[0][0] = a; m.c[0][1] = 0; m.c[0][2] = 0; m.c[0][3] = 0;
		m.c[1][0] = 0; m.c[1][1] = b; m.c[1][2] = 0; m.c[1][3] = 0;
		m.c[2][0] = c; m.c[2][1] = d; m.c[2][2] = -1; m.c[2][3] = -1;
		m.c[3][0] = 0; m.c[3][1] = 0; m.c[3][2] = e; m.c[3][3] = 0;
		return m;
	}

	static Matrix4<T> Orthographic(T left, T right, T bottom, T top, T _near, T _far)
	{
		T a = 2 / (right - left);
		T b = 2 / (top - bottom);
		T c = - (right + left) / (right - left);
		T d = - (top + bottom) / (top - bottom);
		T e = - (_far + _near) / (_far - _near);
		T f = -2 / (_far - _near);
		Matrix4 m;
		m.c[0][0] = a; m.c[0][1] = 0; m.c[0][2] = 0; m.c[0][3] = 0;
		m.c[1][0] = 0; m.c[1][1] = b; m.c[1][2] = 0; m.c[1][3] = 0;
		m.c[2][0] = 0; m.c[2][1] = 0; m.c[2][2] = f; m.c[2][3] = 0;
		m.c[3][0] = c; m.c[3][1] = d; m.c[3][2] = e; m.c[3][3] = 1;
		return m;
	}

	union {
		float c[4][4];	// Column major order for OpenGL: c[column][row]
		float x[16];
	};
};

typedef Matrix2<float> mat2;
typedef Matrix3<float> mat3;
typedef Matrix4<float> mat4;
