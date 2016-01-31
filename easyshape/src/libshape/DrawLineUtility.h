#ifndef _LIBSHAPE_DRAW_LINE_UTILITY_H_
#define _LIBSHAPE_DRAW_LINE_UTILITY_H_



namespace libshape
{

class DrawLineUtility
{
public:

	static bool IsStraightOpen(const std::vector<ee::Vector>& lines, const ee::KeysState& key_state);

	static ee::Vector FixPosTo8DirStraight(const ee::Vector& last, const ee::Vector& curr);

}; // DrawLineUtility

}

#endif // _LIBSHAPE_DRAW_LINE_UTILITY_H_