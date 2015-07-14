#ifndef _LIBSHAPE_DRAW_LINE_UTILITY_H_
#define _LIBSHAPE_DRAW_LINE_UTILITY_H_

#include <drag2d.h>

namespace libshape
{

class DrawLineUtility
{
public:

	static bool IsStraightOpen(const std::vector<d2d::Vector>& lines, const d2d::KeysState& key_state);

	static d2d::Vector FixPosTo8DirStraight(const d2d::Vector& last, const d2d::Vector& curr);

}; // DrawLineUtility

}

#endif // _LIBSHAPE_DRAW_LINE_UTILITY_H_