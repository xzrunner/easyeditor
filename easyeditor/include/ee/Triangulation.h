#ifndef _EASYEDITOR_TRIANGULATION_H_
#define _EASYEDITOR_TRIANGULATION_H_

#include "Vector.h"

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
	static void Normal(const std::vector<Vector>& bound, std::vector<Vector>& result, 
		Type type = e_Constrained);

	static void Holes(const std::vector<Vector>& bound, const std::vector<std::vector<Vector> >& holes,
		std::vector<Vector>& result, Type type = e_Constrained);
	static void HolesNew(const std::vector<Vector>& bound, const std::vector<Vector>& hole,
		std::vector<Vector>& result, Type type = e_Constrained);

	static void Points(const std::vector<Vector>& bound, const std::vector<Vector>& points,
		std::vector<Vector>& result, Type type = e_Constrained);

	static void Lines(const std::vector<Vector>& bound, const std::vector<Vector>& lines,
		std::vector<Vector>& result, Type type = e_Constrained);

	static void PointsAndLines(const std::vector<Vector>& bound, const std::vector<Vector>& points,
		const std::vector<Vector>& lines, std::vector<Vector>& result, Type type = e_Constrained);

	static void LinesAndLoops(const std::vector<Vector>& bound, const std::vector<Vector>& lines,
		const std::vector<std::vector<Vector> >& loops, std::vector<Vector>& result, 
		Type type = e_Constrained);

	static void Strips(const std::vector<Vector>& triangulates, 
		std::vector<std::vector<Vector> >& strips);

private:
	static void VerifyBound(const std::vector<Vector>& src, std::vector<Vector>& dst);

}; // Triangulation

}

#endif // _EASYEDITOR_TRIANGULATION_H_