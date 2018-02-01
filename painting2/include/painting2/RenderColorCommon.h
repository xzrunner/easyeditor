#pragma once

#include "painting2/Color.h"

namespace pt2
{

class RenderColorCommon
{
public:
	pt2::Color mul, add;

public:
	RenderColorCommon();
	RenderColorCommon(const pt2::Color& mul, const pt2::Color& add);

	RenderColorCommon operator * (const RenderColorCommon& rc) const;

	bool operator == (const RenderColorCommon& rc) const;

	void Reset();

	static void Mul(const RenderColorCommon& c0, const RenderColorCommon& c1, RenderColorCommon& c);

}; // RenderColorCommon

}