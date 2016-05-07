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
//#ifndef _EASYEDITOR_MATRIX_H_
//#define _EASYEDITOR_MATRIX_H_
//
//// LOVE
//#include "math.h"
//
//namespace ee
//{
//
///**
//* This class is the basis for all transformations in LOVE. Althought not
//* really needed for 2D, it contains 4x4 elements to be compatible with
//* OpenGL without conversions.
//**/
//class sm::mat4
//{
//private:
//
//	/**
//	* | e0 e4 e8  e12 |
//	* | e1 e5 e9  e13 |
//	* | e2 e6 e10 e14 |
//	* | e3 e7 e11 e15 |
//	**/
//	float e[16];
//
//public:
//
//	/**
//	* Creates a new identity matrix.
//	**/
//	sm::mat4();
//
//	/**
//	* Destructor.
//	**/
//	~sm::mat4();
//
//	/**
//	* Multiplies this sm::mat4 with another sm::mat4, changing neither.
//	* @param m The sm::mat4 to multiply with this sm::mat4.
//	* @return The combined matrix.
//	**/
//	sm::mat4 operator * (const sm::mat4 & m) const;
//
//	/**
//	* Multiplies a sm::mat4 into this sm::mat4.
//	* @param m The sm::mat4 to combine into this sm::mat4.
//	**/
//	void operator *= (const sm::mat4 & m);
//
//	/**
//	* Gets a pointer to the 16 array elements.
//	* @return The array elements.
//	**/
//	const float * GetElements() const;
//
//	/**
//	* Resets this sm::mat4 to the identity matrix.
//	**/
//	void SetIdentity();
//
//	/**
//	* Resets this sm::mat4 to a translation.
//	* @param x Translation along x-axis.
//	* @param y Translation along y-axis.
//	**/
//	void SetTranslation(float x, float y);
//
//	/**
//	* Resets this sm::mat4 to a rotation.
//	* @param r The angle in radians.
//	**/
//	void SetRotation(float r);
//
//	/**
//	* Resets this sm::mat4 to a scale transformation.
//	* @param sx Scale factor along the x-axis.
//	* @param sy Scale factor along the y-axis.
//	**/
//	void SetScale(float sx, float sy);
//
//	/**
//	* Resets this sm::mat4 to a shear transformation.
//	* @param kx Shear along x-axis.
//	* @param ky Shear along y-axis.
//	**/
//	void SetShear(float kx, float ky);
//
//	/**
//	* Creates a transformation with a certain position, orientation, scale
//	* and offset. Perfect for Drawables -- what a coincidence!
//	*
//	* @param x The translation along the x-axis.
//	* @param y The translation along the y-axis.
//	* @param angle The rotation (rad) around the center with offset (ox,oy).
//	* @param sx Scale along x-axis.
//	* @param sy Scale along y-axis.
//	* @param ox The offset for rotation along the x-axis.
//	* @param oy The offset for rotation along the y-axis.
//	* @param kx Shear along x-axis
//	* @param ky Shear along y-axis
//	**/
//	void SetTransformation(float x, float y, float angle, float sx, float sy, float ox, float oy, float kx, float ky);
//
//	/**
//	* Multiplies this sm::mat4 with a translation.
//	* @param x Translation along x-axis.
//	* @param y Translation along y-axis.
//	**/
//	void Translate(float x, float y);
//
//	/**
//	* Multiplies this sm::mat4 with a rotation.
//	* @param r Angle in radians.
//	**/
//	void Rotate(float r);
//
//	/**
//	* Multiplies this sm::mat4 with a scale transformation.
//	* @param sx Scale factor along the x-axis.
//	* @param sy Scale factor along the y-axis.
//	**/
//	void Scale(float sx, float sy);
//
//	/**
//	* Multiplies this sm::mat4 with a shear transformation.
//	* @param kx Shear along the x-axis.
//	* @param ky Shear along the y-axis.
//	**/
//	void Shear(float kx, float ky);
//
//	sm::mat4 GetInvert() const;
//
//	void Orthographic(float left, float right, float bottom, float top, float _near, float _far);
//
//	/**
//	* Transforms an array of vertices by this sm::mat4. The sources and
//	* destination arrays may be the same.
//	*
//	* @param dst Storage for the transformed vertices.
//	* @param src The source vertices.
//	* @param size The number of vertices.
//	**/
////		void transform(vertex * dst, const vertex * src, int size) const;
//
//}; // sm::mat4
//
//}
//
//#endif// _EASYEDITOR_MATRIX_H_
