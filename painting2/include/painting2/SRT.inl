#pragma once

#include <SM_Calc.h>

namespace pt2
{

inline
SRT::SRT() 
{
	Reset();
}

inline
SRT SRT::operator + (const SRT& srt) const
{
	SRT ret;

	ret.position = position + srt.position;
	ret.angle    = angle + srt.angle;
	ret.scale    = scale + srt.scale;
	ret.shear    = shear + srt.shear;

	ret.offset   = offset + srt.offset;
	Update();

	return ret;
}

inline
SRT SRT::operator - (const SRT& srt) const
{
	SRT ret;

	ret.position = position - srt.position;
	ret.angle    = angle - srt.angle;
	ret.scale    = scale - srt.scale;
	ret.shear    = shear - srt.shear;

	ret.offset   = offset - srt.offset;
	Update();

	return ret;
}

inline
SRT SRT::operator * (float f) const
{
	SRT ret;

	ret.position = position * f;
	ret.angle    = angle * f;
	ret.scale    = scale * f;
	ret.shear    = shear * f;

	ret.offset   = offset * f;
	Update();

	return ret;
}

inline
SRT SRT::operator / (float f) const
{
	SRT ret;

	ret.position = position / f;
	ret.angle    = angle / f;
	ret.scale    = scale / f;
	ret.shear    = shear / f;

	ret.offset   = offset / f;
	Update();

	return ret;
}

inline
void SRT::Reset() 
{
	position.Set(0, 0);
	angle = 0;
	scale.Set(1, 1);
	shear.Set(0, 0);
	offset.Set(0, 0);
	center.Set(0, 0);
}

inline
void SRT::Update() const
{
	center = position + sm::rotate_vector(-offset, angle) + offset;
}

}