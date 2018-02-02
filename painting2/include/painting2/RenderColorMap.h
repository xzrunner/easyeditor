#pragma once
#pragma once

#include "painting2/Color.h"

namespace pt2
{

class RenderColorMap
{
public:
	Color rmap, gmap, bmap;

public:
	RenderColorMap();
	RenderColorMap(const Color& rmap, const Color& gmap, const Color& bmap);

	RenderColorMap operator * (const RenderColorMap& rc) const;

	bool operator == (const RenderColorMap& rc) const;

	void Reset();

	static void Mul(const RenderColorMap& c0, const RenderColorMap& c1, RenderColorMap& c);

}; // RenderColorMap

}