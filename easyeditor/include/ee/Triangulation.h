#ifndef _EASYEDITOR_TRIANGULATION_H_
#define _EASYEDITOR_TRIANGULATION_H_

#include <SM_Vector.h>

#include <vector>

namespace ee
{

class Triangulation
{
public:
	enum Type
	{
		e_Constrained,
		e_Conforming,
		e_ConstrainedConformingAngle,
		e_ConstrainedConformingArea,
		e_ConstrainedConformingCount
	};

public:
	static void Normal(const std::vector<sm::vec2>& bound, std::vector<sm::vec2>& result, 
		Type type = e_Constrained);

	static void Holes(const std::vector<sm::vec2>& bound, const std::vector<std::vector<sm::vec2> >& holes,
		std::vector<sm::vec2>& result, Type type = e_Constrained);
	static void HolesNew(const std::vector<sm::vec2>& bound, const std::vector<sm::vec2>& hole,
		std::vector<sm::vec2>& result, Type type = e_Constrained);

	static void Points(const std::vector<sm::vec2>& bound, const std::vector<sm::vec2>& points,
		std::vector<sm::vec2>& result, Type type = e_Constrained);

	static void Lines(const std::vector<sm::vec2>& bound, const std::vector<sm::vec2>& lines,
		std::vector<sm::vec2>& result, Type type = e_Constrained);

	static void PointsAndLines(const std::vector<sm::vec2>& bound, const std::vector<sm::vec2>& points,
		const std::vector<sm::vec2>& lines, std::vector<sm::vec2>& result, Type type = e_Constrained);

	static void LinesAndLoops(const std::vector<sm::vec2>& bound, const std::vector<sm::vec2>& lines,
		const std::vector<std::vector<sm::vec2> >& loops, std::vector<sm::vec2>& result, 
		Type type = e_Constrained);

	static void Strips(const std::vector<sm::vec2>& triangulates, 
		std::vector<std::vector<sm::vec2> >& strips);

private:
	static void VerifyBound(const std::vector<sm::vec2>& src, std::vector<sm::vec2>& dst);

}; // Triangulation

}

#endif // _EASYEDITOR_TRIANGULATION_H_