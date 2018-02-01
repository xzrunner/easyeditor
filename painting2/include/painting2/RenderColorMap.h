#pragma once
#pragma once

#include "painting2/Color.h"

namespace pt2
{

class RenderColorMap
{
public:
	pt2::Color rmap, gmap, bmap;

public:
	RenderColorMap();
	RenderColorMap(const pt2::Color& rmap, const pt2::Color& gmap, const pt2::Color& bmap);

	RenderColorMap operator * (const RenderColorMap& rc) const;

	bool operator == (const RenderColorMap& rc) const;

	void Reset();

	static void Mul(const RenderColorMap& c0, const RenderColorMap& c1, RenderColorMap& c);

}; // RenderColorMap

}