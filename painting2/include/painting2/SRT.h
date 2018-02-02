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
	mutable sm::vec2 center;

public:
	SRT();

	SRT operator + (const SRT& srt) const;
	SRT operator - (const SRT& srt) const;
	SRT operator * (float f) const;
	SRT operator / (float f) const;

	void Reset();

	void Update() const;

}; // SRT

}

#include "painting2/SRT.inl"