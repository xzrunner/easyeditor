#ifndef _DRAG2D_TRIANGULATION_H_
#define _DRAG2D_TRIANGULATION_H_

#include <vector>

#include "common/Vector.h"

namespace d2d
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
	static void normal(const std::vector<Vector>& bound, std::vector<Vector>& result, 
		Type type = e_Constrained);

	static void points(const std::vector<Vector>& bound, const std::vector<Vector>& points,
		std::vector<Vector>& result, Type type = e_Constrained);

	static void lines(const std::vector<Vector>& bound, const std::vector<Vector>& lines,
		std::vector<Vector>& result, Type type = e_Constrained);

	static void pointsAndLines(const std::vector<Vector>& bound, const std::vector<Vector>& points,
		const std::vector<Vector>& lines, std::vector<Vector>& result, Type type = e_Constrained);

	static void linesAndLoops(const std::vector<Vector>& bound, const std::vector<Vector>& lines,
		const std::vector<std::vector<Vector> >& loops, std::vector<Vector>& result, 
		Type type = e_Constrained);

	static void strips(const std::vector<Vector>& triangulates, 
		std::vector<std::vector<Vector> >& strips);

private:
	static void VerifyBound(const std::vector<Vector>& src, std::vector<Vector>& dst);

}; // Triangulation

}

#endif // _DRAG2D_TRIANGULATION_H_