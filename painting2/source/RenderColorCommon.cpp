#include "painting2/RenderColorCommon.h"

namespace pt2
{

static const Color COL_ONE(255, 255, 255, 255);
static const Color COL_ZERO(0, 0, 0, 0);
static const float INV_255 = 1.0f / 255;

RenderColorCommon::RenderColorCommon()
	: mul(255, 255, 255, 255)
	, add(0, 0, 0, 0)
{
}

RenderColorCommon::RenderColorCommon(const Color& mul, const Color& add)
	: mul(mul)
	, add(add)
{
}

RenderColorCommon RenderColorCommon::operator * (const RenderColorCommon& rc) const
{
	RenderColorCommon ret;
	Mul(*this, rc, ret);
	return ret;
}

bool RenderColorCommon::operator == (const RenderColorCommon& rc) const
{
	return rc.mul == mul && rc.add == add;
}

void RenderColorCommon::Reset()
{
	mul.FromRGBA(0xffffffff);
	add.FromRGBA(0);
}

void RenderColorCommon::Mul(const RenderColorCommon& c0, const RenderColorCommon& c1, RenderColorCommon& c)
{
	// mul
	auto& mul0 = c0.mul;
	auto& mul1 = c1.mul;
	auto& mul = c.mul;
	if (mul0 == COL_ONE) {
		mul = mul1;
	} else if (mul1 == COL_ONE) {
		mul = mul0;
	} else if (mul0 == COL_ZERO || mul1 == COL_ZERO) {
		mul = COL_ZERO;
	} else {
		mul.r = static_cast<uint8_t>(mul0.r * mul1.r * INV_255);
		mul.g = static_cast<uint8_t>(mul0.g * mul1.g * INV_255);
		mul.b = static_cast<uint8_t>(mul0.b * mul1.b * INV_255);
		mul.a = static_cast<uint8_t>(mul0.a * mul1.a * INV_255);
	}

	// add
	auto& add0 = c0.add;
	auto& add1 = c1.add;
	auto& add = c.add;
	if (add0 == COL_ZERO) {
		add = add1;
	} else if (add1 == COL_ZERO) {
		add = add0;
	} else {
		add.r = clamp(add0.r + add1.r);
		add.g = clamp(add0.g + add1.g);
		add.b = clamp(add0.b + add1.b);
		add.a = clamp(add0.a + add1.a);
	}
}

}