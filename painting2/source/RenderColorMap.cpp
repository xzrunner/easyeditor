#include "painting2/RenderColorMap.h"

namespace pt2
{

static const Color R_IDENTITY(255, 0, 0, 0);
static const Color G_IDENTITY(0, 255, 0, 0);
static const Color B_IDENTITY(0, 0, 255, 0);
static const float INV_255 = 1.0f / 255;

RenderColorMap::RenderColorMap()
	: rmap(255, 0, 0, 0)
	, gmap(0, 255, 0, 0)
	, bmap(0, 0, 255, 0)
{
}

RenderColorMap::RenderColorMap(const Color& rmap, const Color& gmap, const Color& bmap)
	: rmap(rmap)
	, gmap(gmap)
	, bmap(bmap)
{
}

RenderColorMap RenderColorMap::operator * (const RenderColorMap& rc) const
{
	RenderColorMap ret;
	Mul(*this, rc, ret);
	return ret;
}

bool RenderColorMap::operator == (const RenderColorMap& rc) const
{
	return rmap == rc.rmap
		&& gmap == rc.gmap
		&& bmap == rc.bmap;
}

void RenderColorMap::Reset()
{
	rmap.FromRGBA(0xff000000);
	gmap.FromRGBA(0x00ff0000);
	bmap.FromRGBA(0x0000ff00);
}

void RenderColorMap::Mul(const RenderColorMap& c0, const RenderColorMap& c1, RenderColorMap& c)
{
	auto& r0 = c0.rmap;
	auto& g0 = c0.gmap;
	auto& b0 = c0.bmap;
	auto& r1 = c1.rmap;
	auto& g1 = c1.gmap;
	auto& b1 = c1.bmap;
	if (r0 == R_IDENTITY && g0 == G_IDENTITY && b0 == B_IDENTITY) {
		c = c1;
	} else if (r1 == R_IDENTITY && g1 == G_IDENTITY && b1 == B_IDENTITY) {
		c = c0;
	} else {
		auto& r = c.rmap;
		auto& g = c.gmap;
		auto& b = c.bmap;

		r.r = static_cast<uint8_t>((r0.r * r1.r + r0.g * g1.r + r0.b * b1.r) * INV_255);
		r.g = static_cast<uint8_t>((r0.r * r1.g + r0.g * g1.g + r0.b * b1.g) * INV_255);
		r.b = static_cast<uint8_t>((r0.r * r1.b + r0.g * g1.b + r0.b * b1.b) * INV_255);
		r.a = 0;

		g.r = static_cast<uint8_t>((g0.r * r1.r + g0.g * g1.r + g0.b * b1.r) * INV_255);
		g.g = static_cast<uint8_t>((g0.r * r1.g + g0.g * g1.g + g0.b * b1.g) * INV_255);
		g.b = static_cast<uint8_t>((g0.r * r1.b + g0.g * g1.b + g0.b * b1.b) * INV_255);
		g.a = 0;

		b.r = static_cast<uint8_t>((b0.r * r1.r + b0.g * g1.r + b0.b * b1.r) * INV_255);
		b.g = static_cast<uint8_t>((b0.r * r1.g + b0.g * g1.g + b0.b * b1.g) * INV_255);
		b.b = static_cast<uint8_t>((b0.r * r1.b + b0.g * g1.b + b0.b * b1.b) * INV_255);
		b.a = 0;
	}
}

}