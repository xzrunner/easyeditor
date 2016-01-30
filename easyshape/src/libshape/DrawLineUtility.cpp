#include "DrawLineUtility.h"

namespace libshape
{

bool DrawLineUtility::IsStraightOpen(const std::vector<d2d::Vector>& lines, 
									 const d2d::KeysState& key_state)
{
	return !lines.empty() && key_state.GetKeyState(WXK_SHIFT);
}

d2d::Vector DrawLineUtility::FixPosTo8DirStraight(const d2d::Vector& last, 
												  const d2d::Vector& curr)
{
	float nearest;
	d2d::Vector fixed = curr;

	const float dx = fabs(curr.x - last.x);
	nearest = dx;
	fixed.Set(last.x, curr.y);

	const float dy = fabs(curr.y - last.y);
	if (dy < nearest)
	{
		nearest = dy;
		fixed.Set(curr.x, last.y);
	}

	d2d::Vector other(last.x + 1, last.y - 1);
	const float dxyDown = d2d::Math2D::GetDisPointToStraightLine(curr, last, other);
	if (dxyDown < nearest)
	{
		nearest = dxyDown;
		d2d::Math2D::GetFootOfPerpendicular(last, other, curr, &fixed);
	}

	other.Set(last.x + 1, last.y + 1);
	const float dxyUp = d2d::Math2D::GetDisPointToStraightLine(curr, last, other);
	if (dxyUp < nearest)
	{
		nearest = dxyUp;
		d2d::Math2D::GetFootOfPerpendicular(last, other, curr, &fixed);
	}

	return fixed;
}

}