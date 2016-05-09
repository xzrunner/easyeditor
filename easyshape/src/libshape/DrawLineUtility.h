#ifndef _EASYSHAPE_DRAW_LINE_UTILITY_H_
#define _EASYSHAPE_DRAW_LINE_UTILITY_H_

#include <ee/KeysState.h>

#include <SM_Vector.h>

#include <vector>

namespace eshape
{

class DrawLineUtility
{
public:

	static bool IsStraightOpen(const std::vector<sm::vec2>& lines, const ee::KeysState& key_state);

	static sm::vec2 FixPosTo8DirStraight(const sm::vec2& last, const sm::vec2& curr);

}; // DrawLineUtility

}

#endif // _EASYSHAPE_DRAW_LINE_UTILITY_H_