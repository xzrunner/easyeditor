#pragma once

#include <SM_Vector.h>

namespace pt2
{

class SRT
{
public:
	sm::vec2 position;
	float    angle;
	sm::vec2 scale;
	sm::vec2 shear;

	sm::vec2 offset;
	sm::vec2 center;

	SRT() {
		Reset();
	}

	void Reset() {
		position.Set(0, 0);
		angle = 0;
		scale.Set(1, 1);
		shear.Set(0, 0);
		offset.Set(0, 0);
		center.Set(0, 0);
	}

}; // SRT

}