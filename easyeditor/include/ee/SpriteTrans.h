#ifndef _EASYEDITOR_SPRITE_TRANS_H_
#define _EASYEDITOR_SPRITE_TRANS_H_

#include "Matrix.h"
#include "ColorTrans.h"
#include "ShaderTrans.h"

namespace ee
{

class SpriteTrans
{
public:
	SpriteTrans() {}
	SpriteTrans(const Matrix& mt) : mt(mt) {}
	SpriteTrans(const Matrix& mt, const ColorTrans& color) 
		: mt(mt)
		, color(color)
	{}

public:
	Matrix mt;

	ColorTrans color;

	ShaderTrans shader;

}; // SpriteTrans

}

#endif // _EASYEDITOR_SPRITE_TRANS_H_
