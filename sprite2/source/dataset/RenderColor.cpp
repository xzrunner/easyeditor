#include "RenderColor.h"

namespace s2
{

RenderColor RenderColor::operator * (const RenderColor& rc) const
{
	RenderColor ret;
	ret.mul = mul * rc.mul;
	ret.add = add + rc.add;
	ret.rmap = rmap.MapMul(rc.rmap, rc.gmap, rc.bmap);
	ret.gmap = gmap.MapMul(rc.rmap, rc.gmap, rc.bmap);
	ret.bmap = bmap.MapMul(rc.rmap, rc.gmap, rc.bmap);
	return ret;
}

}