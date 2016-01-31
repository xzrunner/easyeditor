#include "DrawLineUtility.h"

namespace libshape
{

bool DrawLineUtility::IsStraightOpen(const std::vector<ee::Vector>& lines, 
									 const ee::KeysState& key_state)
{
	return !lines.empty() && key_state.GetKeyState(WXK_SHIFT);
}

ee::Vector DrawLineUtility::FixPosTo8DirStraight(const ee::Vector& last, 
												  const ee::Vector& curr)
{
	float nearest;
	ee::Vector fixed = curr;

	const float dx = fabs(curr.x - last.x);
	nearest = dx;
	fixed.Set(last.x, curr.y);

	const float dy = fabs(curr.y - last.y);
	if (dy < nearest)
	{
		nearest = dy;
		fixed.Set(curr.x, last.y);
	}

	ee::Vector other(last.x + 1, last.y - 1);
	const float dxyDown = ee::Math2D::GetDisPointToStraightLine(curr, last, other);
	if (dxyDown < nearest)
	{
		nearest = dxyDown;
		ee::Math2D::GetFootOfPerpendicular(last, other, curr, &fixed);
	}

	other.Set(last.x + 1, last.y + 1);
	const float dxyUp = ee::Math2D::GetDisPointToStraightLine(curr, last, other);
	if (dxyUp < nearest)
	{
		nearest = dxyUp;
		ee::Math2D::GetFootOfPerpendicular(last, other, curr, &fixed);
	}

	return fixed;
}

}