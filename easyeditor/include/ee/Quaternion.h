#ifndef _EASYEDITOR_QUATERNION_H_
#define _EASYEDITOR_QUATERNION_H_

#include "Matrix3D.h"

namespace ee
{

template <typename T>
struct QuaternionT {
	T x;
	T y;
	T z;
	T w;

	QuaternionT();
	QuaternionT(T x, T y, T z, T w);

	QuaternionT<T> Slerp(T mu, const QuaternionT<T>& q) const;
	QuaternionT<T> Rotated(const QuaternionT<T>& b) const;
	QuaternionT<T> Scaled(T scale) const;
	T Dot(const QuaternionT<T>& q) const;
	Matrix3<T> ToMatrix() const;
	Vector4<T> ToVector() const;
	QuaternionT<T> operator-(const QuaternionT<T>& q) const;
	QuaternionT<T> operator+(const QuaternionT<T>& q) const;
	bool operator==(const QuaternionT<T>& q) const;
	bool operator!=(const QuaternionT<T>& q) const;

	void Normalize();
	void Rotate(const QuaternionT<T>& q);

	static QuaternionT<T> CreateFromVectors(const Vector3<T>& v0, const Vector3<T>& v1);
	static QuaternionT<T> CreateFromAxisAngle(const Vector3<T>& axis, float radians);
};

typedef QuaternionT<float> Quaternion;

}

#include "Quaternion.inl"

#endif // _EASYEDITOR_QUATERNION_H_