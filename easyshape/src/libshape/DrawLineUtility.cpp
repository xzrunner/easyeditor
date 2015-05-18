#include "DrawLineUtility.h"

namespace libshape
{

bool DrawLineUtility::IsStraightOpen(const std::vector<d2d::Vector>& lines)
{
	return !lines.empty() && wxGetKeyState(WXK_SHIFT);
}

d2d::Vector DrawLineUtility::FixPosTo8DirStraight(const d2d::Vector& last, 
												  const d2d::Vector& curr)
{
	float nearest;
	d2d::Vector fixed = curr;

	const float dx = fabs(curr.x - last.x);
	nearest = dx;
	fixed.set(last.x, curr.y);

	const float dy = fabs(curr.y - last.y);
	if (dy < nearest)
	{
		nearest = dy;
		fixed.set(curr.x, last.y);
	}

	d2d::Vector other(last.x + 1, last.y - 1);
	const float dxyDown = d2d::Math::getDisPointToStraightLine(curr, last, other);
	if (dxyDown < nearest)
	{
		nearest = dxyDown;
		d2d::Math::getFootOfPerpendicular(last, other, curr, &fixed);
	}

	other.set(last.x + 1, last.y + 1);
	const float dxyUp = d2d::Math::getDisPointToStraightLine(curr, last, other);
	if (dxyUp < nearest)
	{
		nearest = dxyUp;
		d2d::Math::getFootOfPerpendicular(last, other, curr, &fixed);
	}

	return fixed;
}

}