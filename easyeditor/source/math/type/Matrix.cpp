///**
//* Copyright (c) 2006-2012 LOVE Development Team
//*
//* This software is provided 'as-is', without any express or implied
//* warranty.  In no event will the authors be held liable for any damages
//* arising from the use of this software.
//*
//* Permission is granted to anyone to use this software for any purpose,
//* including commercial applications, and to alter it and redistribute it
//* freely, subject to the following restrictions:
//*
//* 1. The origin of this software must not be misrepresented; you must not
//*    claim that you wrote the original software. If you use this software
//*    in a product, an acknowledgment in the product documentation would be
//*    appreciated but is not required.
//* 2. Altered source versions must be plainly marked as such, and must not be
//*    misrepresented as being the original software.
//* 3. This notice may not be removed or altered from any source distribution.
//**/
//
//#include "sm::mat4.h"
//
//// STD
//#include <cstring> // memcpy
//#include <cmath>
//
//namespace ee
//{
//
//// | e0 e4 e8  e12 |
//// | e1 e5 e9  e13 |
//// | e2 e6 e10 e14 |
//// | e3 e7 e11 e15 |
//
//sm::mat4::sm::mat4()
//{
//	SetIdentity();
//}
//
//sm::mat4::~sm::mat4()
//{
//}
//
////                 | e0 e4 e8  e12 |
////                 | e1 e5 e9  e13 |
////                 | e2 e6 e10 e14 |
////                 | e3 e7 e11 e15 |
//// | e0 e4 e8  e12 |
//// | e1 e5 e9  e13 |
//// | e2 e6 e10 e14 |
//// | e3 e7 e11 e15 |
//
//sm::mat4 sm::mat4::operator * (const sm::mat4 & m) const
//{
//	sm::mat4 t;
//
//	t.e[0] = (e[0]*m.e[0]) + (e[4]*m.e[1]) + (e[8]*m.e[2]) + (e[12]*m.e[3]);
//	t.e[4] = (e[0]*m.e[4]) + (e[4]*m.e[5]) + (e[8]*m.e[6]) + (e[12]*m.e[7]);
//	t.e[8] = (e[0]*m.e[8]) + (e[4]*m.e[9]) + (e[8]*m.e[10]) + (e[12]*m.e[11]);
//	t.e[12] = (e[0]*m.e[12]) + (e[4]*m.e[13]) + (e[8]*m.e[14]) + (e[12]*m.e[15]);
//
//	t.e[1] = (e[1]*m.e[0]) + (e[5]*m.e[1]) + (e[9]*m.e[2]) + (e[13]*m.e[3]);
//	t.e[5] = (e[1]*m.e[4]) + (e[5]*m.e[5]) + (e[9]*m.e[6]) + (e[13]*m.e[7]);
//	t.e[9] = (e[1]*m.e[8]) + (e[5]*m.e[9]) + (e[9]*m.e[10]) + (e[13]*m.e[11]);
//	t.e[13] = (e[1]*m.e[12]) + (e[5]*m.e[13]) + (e[9]*m.e[14]) + (e[13]*m.e[15]);
//
//	t.e[2] = (e[2]*m.e[0]) + (e[6]*m.e[1]) + (e[10]*m.e[2]) + (e[14]*m.e[3]);
//	t.e[6] = (e[2]*m.e[4]) + (e[6]*m.e[5]) + (e[10]*m.e[6]) + (e[14]*m.e[7]);
//	t.e[10] = (e[2]*m.e[8]) + (e[6]*m.e[9]) + (e[10]*m.e[10]) + (e[14]*m.e[11]);
//	t.e[14] = (e[2]*m.e[12]) + (e[6]*m.e[13]) + (e[10]*m.e[14]) + (e[14]*m.e[15]);
//
//	t.e[3] = (e[3]*m.e[0]) + (e[7]*m.e[1]) + (e[11]*m.e[2]) + (e[15]*m.e[3]);
//	t.e[7] = (e[3]*m.e[4]) + (e[7]*m.e[5]) + (e[11]*m.e[6]) + (e[15]*m.e[7]);
//	t.e[11] = (e[3]*m.e[8]) + (e[7]*m.e[9]) + (e[11]*m.e[10]) + (e[15]*m.e[11]);
//	t.e[15] = (e[3]*m.e[12]) + (e[7]*m.e[13]) + (e[11]*m.e[14]) + (e[15]*m.e[15]);
//
//	return t;
//}
//
//void sm::mat4::operator *= (const sm::mat4 & m)
//{
//	sm::mat4 t = (*this) * m;
//	memcpy((void*)this->e, (void*)t.e, sizeof(float)*16);
//}
//
//const float * sm::mat4::GetElements() const
//{
//	return e;
//}
//
//void sm::mat4::SetIdentity()
//{
//	memset(e, 0, sizeof(float)*16);
//	e[0] = e[5] = e[10] = e[15] = 1;
//}
//
//void sm::mat4::SetTranslation(float x, float y)
//{
//	SetIdentity();
//	e[12] = x;
//	e[13] = y;
//}
//
//void sm::mat4::SetRotation(float rad)
//{
//	SetIdentity();
//	float c = cos(rad), s = sin(rad);
//	e[0] = c; e[4] = -s;
//	e[1] = s; e[5] = c;
//}
//
//void sm::mat4::SetScale(float sx, float sy)
//{
//	SetIdentity();
//	e[0] = sx;
//	e[5] = sy;
//}
//
//void sm::mat4::SetShear(float kx, float ky)
//{
//	SetIdentity();
//	e[1] = ky;
//	e[4] = kx;
//}
//
//void sm::mat4::SetTransformation(float x, float y, float angle, float sx, float sy, float ox, float oy, float kx, float ky)
//{
//	memset(e, 0, sizeof(float)*16); // zero out matrix
//	float c = cos(angle), s = sin(angle);
//	// matrix multiplication carried out on paper:
//	// |1     x| |c -s    | |sx       | | 1 ky    | |1     -ox|
//	// |  1   y| |s  c    | |   sy    | |kx  1    | |  1   -oy|
//	// |    1  | |     1  | |      1  | |      1  | |    1    |
//	// |      1| |       1| |        1| |        1| |       1 |
//	//   move      rotate      scale       skew       origin
//	e[10] = e[15] = 1.0f;
//	e[0]  = c * sx - ky * s * sy; // = a
//	e[1]  = s * sx + ky * c * sy; // = b
//	e[4]  = kx * c * sx - s * sy; // = c
//	e[5]  = kx * s * sx + c * sy; // = d
//	e[12] = x - ox * e[0] - oy * e[4];
//	e[13] = y - ox * e[1] - oy * e[5];
//}
//
//void sm::mat4::Translate(float x, float y)
//{
//	sm::mat4 t;
//	t.SetTranslation(x, y);
//	this->operator *=(t);
//}
//
//void sm::mat4::Rotate(float rad)
//{
//	sm::mat4 t;
//	t.SetRotation(rad);
//	this->operator *=(t);
//}
//
//void sm::mat4::Scale(float sx, float sy)
//{
//	sm::mat4 t;
//	t.SetScale(sx, sy);
//	this->operator *=(t);
//}
//
//void sm::mat4::Shear(float kx, float ky)
//{
//	sm::mat4 t;
//	t.SetShear(kx,ky);
//	this->operator *=(t);
//}
//
//sm::mat4 sm::mat4::GetInvert() const
//{
//	double inv[16], det;
//	int i;
//
//	inv[0] = e[5]  * e[10] * e[15] - 
//		e[5]  * e[11] * e[14] - 
//		e[9]  * e[6]  * e[15] + 
//		e[9]  * e[7]  * e[14] +
//		e[13] * e[6]  * e[11] - 
//		e[13] * e[7]  * e[10];
//
//	inv[4] = -e[4]  * e[10] * e[15] + 
//		e[4]  * e[11] * e[14] + 
//		e[8]  * e[6]  * e[15] - 
//		e[8]  * e[7]  * e[14] - 
//		e[12] * e[6]  * e[11] + 
//		e[12] * e[7]  * e[10];
//
//	inv[8] = e[4]  * e[9] * e[15] - 
//		e[4]  * e[11] * e[13] - 
//		e[8]  * e[5] * e[15] + 
//		e[8]  * e[7] * e[13] + 
//		e[12] * e[5] * e[11] - 
//		e[12] * e[7] * e[9];
//
//	inv[12] = -e[4]  * e[9] * e[14] + 
//		e[4]  * e[10] * e[13] +
//		e[8]  * e[5] * e[14] - 
//		e[8]  * e[6] * e[13] - 
//		e[12] * e[5] * e[10] + 
//		e[12] * e[6] * e[9];
//
//	inv[1] = -e[1]  * e[10] * e[15] + 
//		e[1]  * e[11] * e[14] + 
//		e[9]  * e[2] * e[15] - 
//		e[9]  * e[3] * e[14] - 
//		e[13] * e[2] * e[11] + 
//		e[13] * e[3] * e[10];
//
//	inv[5] = e[0]  * e[10] * e[15] - 
//		e[0]  * e[11] * e[14] - 
//		e[8]  * e[2] * e[15] + 
//		e[8]  * e[3] * e[14] + 
//		e[12] * e[2] * e[11] - 
//		e[12] * e[3] * e[10];
//
//	inv[9] = -e[0]  * e[9] * e[15] + 
//		e[0]  * e[11] * e[13] + 
//		e[8]  * e[1] * e[15] - 
//		e[8]  * e[3] * e[13] - 
//		e[12] * e[1] * e[11] + 
//		e[12] * e[3] * e[9];
//
//	inv[13] = e[0]  * e[9] * e[14] - 
//		e[0]  * e[10] * e[13] - 
//		e[8]  * e[1] * e[14] + 
//		e[8]  * e[2] * e[13] + 
//		e[12] * e[1] * e[10] - 
//		e[12] * e[2] * e[9];
//
//	inv[2] = e[1]  * e[6] * e[15] - 
//		e[1]  * e[7] * e[14] - 
//		e[5]  * e[2] * e[15] + 
//		e[5]  * e[3] * e[14] + 
//		e[13] * e[2] * e[7] - 
//		e[13] * e[3] * e[6];
//
//	inv[6] = -e[0]  * e[6] * e[15] + 
//		e[0]  * e[7] * e[14] + 
//		e[4]  * e[2] * e[15] - 
//		e[4]  * e[3] * e[14] - 
//		e[12] * e[2] * e[7] + 
//		e[12] * e[3] * e[6];
//
//	inv[10] = e[0]  * e[5] * e[15] - 
//		e[0]  * e[7] * e[13] - 
//		e[4]  * e[1] * e[15] + 
//		e[4]  * e[3] * e[13] + 
//		e[12] * e[1] * e[7] - 
//		e[12] * e[3] * e[5];
//
//	inv[14] = -e[0]  * e[5] * e[14] + 
//		e[0]  * e[6] * e[13] + 
//		e[4]  * e[1] * e[14] - 
//		e[4]  * e[2] * e[13] - 
//		e[12] * e[1] * e[6] + 
//		e[12] * e[2] * e[5];
//
//	inv[3] = -e[1] * e[6] * e[11] + 
//		e[1] * e[7] * e[10] + 
//		e[5] * e[2] * e[11] - 
//		e[5] * e[3] * e[10] - 
//		e[9] * e[2] * e[7] + 
//		e[9] * e[3] * e[6];
//
//	inv[7] = e[0] * e[6] * e[11] - 
//		e[0] * e[7] * e[10] - 
//		e[4] * e[2] * e[11] + 
//		e[4] * e[3] * e[10] + 
//		e[8] * e[2] * e[7] - 
//		e[8] * e[3] * e[6];
//
//	inv[11] = -e[0] * e[5] * e[11] + 
//		e[0] * e[7] * e[9] + 
//		e[4] * e[1] * e[11] - 
//		e[4] * e[3] * e[9] - 
//		e[8] * e[1] * e[7] + 
//		e[8] * e[3] * e[5];
//
//	inv[15] = e[0] * e[5] * e[10] - 
//		e[0] * e[6] * e[9] - 
//		e[4] * e[1] * e[10] + 
//		e[4] * e[2] * e[9] + 
//		e[8] * e[1] * e[6] - 
//		e[8] * e[2] * e[5];
//
//	det = e[0] * inv[0] + e[1] * inv[4] + e[2] * inv[8] + e[3] * inv[12];
//
//	if (det == 0) {
//		sm::mat4 ret;
//		return ret;
//	}
//
//	det = 1.0 / det;
//
//	sm::mat4 ret;
//	for (i = 0; i < 16; i++) {
//		ret.e[i] = static_cast<float>(inv[i] * det);
//	}
//	return ret;
//}
//
//void sm::mat4::Orthographic(float left, float right, float bottom, float top, float _near, float _far)
//{
//	float a = 2 / (right - left);
//	float b = 2 / (top - bottom);
//	float c = - (right + left) / (right - left);
//	float d = - (top + bottom) / (top - bottom);
//	float e = - (_far + _near) / (_far - _near);
//	float f = -2 / (_far - _near);
//	this->e[0] = a; this->e[1] = 0; this->e[2] = 0; this->e[3] = 0;
//	this->e[4] = 0; this->e[5] = b; this->e[6] = 0; this->e[7] = 0;
//	this->e[8] = 0; this->e[9] = 0; this->e[10] = f; this->e[11] = 0;
//	this->e[12] = c; this->e[13] = d; this->e[14] = e; this->e[15] = 1;
//}
//
//// 	//                 | x |
//// 	//                 | y |
//// 	//                 | 0 |
//// 	//                 | 1 |
//// 	// | e0 e4 e8  e12 |
//// 	// | e1 e5 e9  e13 |
//// 	// | e2 e6 e10 e14 |
//// 	// | e3 e7 e11 e15 |
//// 
//// 	void sm::mat4::transform(vertex * dst, const vertex * src, int size) const
//// 	{
//// 		for (int i = 0;i<size;i++)
//// 		{
//// 			// Store in temp variables in case src = dst
//// 			float x = (e[0]*src[i].x) + (e[4]*src[i].y) + (0) + (e[12]);
//// 			float y = (e[1]*src[i].x) + (e[5]*src[i].y) + (0) + (e[13]);
//// 
//// 			dst[i].x = x;
//// 			dst[i].y = y;
//// 		}
//// 	}
//
//
//} // love
