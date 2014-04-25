#include "Triangulation.h"
#include "SGI.h"
#include "common/Math.h"

#include <Triangle/triangle.cpp>
#include <assert.h>

namespace d2d
{

static void init(struct triangulateio& out)
{
	out.pointlist = (REAL *) NULL;
	out.pointattributelist = (REAL *) NULL;
	out.pointmarkerlist = (int *) NULL;
	out.trianglelist = (int *) NULL;
	out.triangleattributelist = (REAL *) NULL;
	out.segmentlist = (int *) NULL;
	out.segmentmarkerlist = (int *) NULL;
}

static void implement(struct triangulateio& in, struct triangulateio& out, Triangulation::Type type)
{
	switch (type)
	{
	case Triangulation::e_Constrained:
		triangulate("pz", &in, &out, (struct triangulateio *) NULL);
		break;
	case Triangulation::e_Conforming:
		triangulate("pzD", &in, &out, (struct triangulateio *) NULL);
		break;
	case Triangulation::e_ConstrainedConformingAngle:
		triangulate("pzq", &in, &out, (struct triangulateio *) NULL);
		break;
	case Triangulation::e_ConstrainedConformingArea:
		triangulate("pza10000", &in, &out, (struct triangulateio *) NULL);
		break;
	case Triangulation::e_ConstrainedConformingCount:
		triangulate("pzu100", &in, &out, (struct triangulateio *) NULL);
		break;
	default:
		assert(0);
	}
}

static void finish(struct triangulateio& out, 
				   const std::vector<Vector>& bound, 
				   std::vector<Vector>& result)
{
	int index = 0;
	for (size_t i = 0; i < out.numberoftriangles; ++i)
	{
		std::vector<Vector> tri;
		for (size_t j = 0; j < out.numberofcorners; ++j)
		{
			int pIndex = out.trianglelist[index++];

			Vector p;
			p.x = out.pointlist[pIndex * 2];
			p.y = out.pointlist[pIndex * 2 + 1];
			tri.push_back(p);
		}

		Vector center = Math::getTriGravityCenter(tri[0], tri[1], tri[2]);
		if (Math::isPointInArea(center, bound))
			copy(tri.begin(), tri.end(), back_inserter(result));
	}
}

void Triangulation::normal(const std::vector<Vector>& bound, 
						   std::vector<Vector>& result, 
						   Type type)
{
	struct triangulateio in, out;

	in.numberofpoints = bound.size();
	in.numberofpointattributes = 0;
	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
	in.pointmarkerlist = (int *) NULL;
	int index = 0;
	for (size_t i = 0; i < in.numberofpoints; ++i)
	{
		in.pointlist[index++] = bound[i].x;
		in.pointlist[index++] = bound[i].y;
	}

	in.numberofsegments = in.numberofpoints;
	in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));
	index = 0;
	for (size_t i = 0; i < in.numberofsegments - 1; ++i)
	{
		in.segmentlist[index++] = i;
		in.segmentlist[index++] = i + 1;
	}
	in.segmentlist[index++] = in.numberofsegments - 1;
	in.segmentlist[index++] = 0;

	in.segmentmarkerlist = (int *) NULL;

	in.numberofholes = 0;
	in.numberofregions = 0;

	init(out);
	implement(in, out, type);
	finish(out, bound, result);
}

void Triangulation::points(const std::vector<Vector>& bound, 
						   const std::vector<Vector>& points,
						   std::vector<Vector>& result, 
						   Type type)
{
	struct triangulateio in, out;

	in.numberofpoints = bound.size() + points.size();
	in.numberofpointattributes = 0;
	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
	in.pointmarkerlist = (int *) NULL;
	int index = 0;
	for (size_t i = 0, n = bound.size(); i < n; ++i)
	{
		in.pointlist[index++] = bound[i].x;
		in.pointlist[index++] = bound[i].y;
	}
	for (size_t i = 0, n = points.size(); i < n; ++i)
	{
		in.pointlist[index++] = points[i].x;
		in.pointlist[index++] = points[i].y;
	}

	in.numberofsegments = bound.size();
	in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));

	index = 0;
	for (size_t i = 0, n = bound.size() - 1; i < n; ++i)
	{
		in.segmentlist[index++] = i;
		in.segmentlist[index++] = i + 1;
	}
	in.segmentlist[index++] = bound.size() - 1;
	in.segmentlist[index++] = 0;

	in.segmentmarkerlist = (int *) NULL;

	in.numberofholes = 0;
	in.numberofregions = 0;

	init(out);
	implement(in, out, type);
	finish(out, bound, result);
}

void Triangulation::lines(const std::vector<Vector>& bound, 
						  const std::vector<Vector>& lines,
						  std::vector<Vector>& result, 
						  Type type)
{
	struct triangulateio in, out;

	in.numberofpoints = bound.size() + lines.size();
	in.numberofpointattributes = 0;
	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
	in.pointmarkerlist = (int *) NULL;
	int index = 0;
	for (size_t i = 0, n = bound.size(); i < n; ++i)
	{
		in.pointlist[index++] = bound[i].x;
		in.pointlist[index++] = bound[i].y;
	}
	for (size_t i = 0, n = lines.size(); i < n; ++i)
	{
		in.pointlist[index++] = lines[i].x;
		in.pointlist[index++] = lines[i].y;
	}

	in.numberofsegments = bound.size() + lines.size() / 2;
	in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));

	index = 0;
	for (size_t i = 0, n = bound.size() - 1; i < n; ++i)
	{
		in.segmentlist[index++] = i;
		in.segmentlist[index++] = i + 1;
	}
	in.segmentlist[index++] = bound.size() - 1;
	in.segmentlist[index++] = 0;

	int lineIndex = bound.size();
	for (size_t i = 0, n = lines.size() / 2; i < n; ++i)
	{
		in.segmentlist[index++] = lineIndex++;
		in.segmentlist[index++] = lineIndex++;
	}

	in.segmentmarkerlist = (int *) NULL;

	in.numberofholes = 0;
	in.numberofregions = 0;

	init(out);
	implement(in, out, type);
	finish(out, bound, result);
}

void Triangulation::pointsAndLines(const std::vector<Vector>& bound, const std::vector<Vector>& points,
								   const std::vector<Vector>& lines, std::vector<Vector>& result, Type type /*= e_Constrained*/)
{
	struct triangulateio in, out;

	in.numberofpoints = bound.size() + lines.size() + points.size();
	in.numberofpointattributes = 0;
	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
	in.pointmarkerlist = (int *) NULL;
	int index = 0;
	for (size_t i = 0, n = bound.size(); i < n; ++i)
	{
		in.pointlist[index++] = bound[i].x;
		in.pointlist[index++] = bound[i].y;
	}
	for (size_t i = 0, n = lines.size(); i < n; ++i)
	{
		in.pointlist[index++] = lines[i].x;
		in.pointlist[index++] = lines[i].y;
	}
	for (size_t i = 0, n = points.size(); i < n; ++i)
	{
		in.pointlist[index++] = points[i].x;
		in.pointlist[index++] = points[i].y;
	}

	in.numberofsegments = bound.size() + lines.size() / 2;
	in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));

	index = 0;
	for (size_t i = 0, n = bound.size() - 1; i < n; ++i)
	{
		in.segmentlist[index++] = i;
		in.segmentlist[index++] = i + 1;
	}
	in.segmentlist[index++] = bound.size() - 1;
	in.segmentlist[index++] = 0;

	int lineIndex = bound.size();
	for (size_t i = 0, n = lines.size() / 2; i < n; ++i)
	{
		in.segmentlist[index++] = lineIndex++;
		in.segmentlist[index++] = lineIndex++;
	}

	in.segmentmarkerlist = (int *) NULL;

	in.numberofholes = 0;
	in.numberofregions = 0;

	init(out);
	implement(in, out, type);
	finish(out, bound, result);
}

void Triangulation::linesAndLoops(const std::vector<Vector>& bound, 
								  const std::vector<Vector>& lines,
								  const std::vector<std::vector<Vector> >& loops, 
								  std::vector<Vector>& result, 
								  Type type)
{
	struct triangulateio in, out;

	size_t loopSize = 0;
	for (size_t i = 0, n = loops.size(); i < n; ++i)
		loopSize += loops[i].size();

	in.numberofpoints = bound.size() + lines.size() + loopSize;
	in.numberofpointattributes = 0;
	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
	in.pointmarkerlist = (int *) NULL;
	int index = 0;
	for (size_t i = 0, n = bound.size(); i < n; ++i)
	{
		in.pointlist[index++] = bound[i].x;
		in.pointlist[index++] = bound[i].y;
	}
	for (size_t i = 0, n = lines.size(); i < n; ++i)
	{
		in.pointlist[index++] = lines[i].x;
		in.pointlist[index++] = lines[i].y;
	}
	for (size_t i = 0, n = loops.size(); i < n; ++i)
	{
		for (size_t j = 0, m = loops[i].size(); j < m; ++j)
		{
			in.pointlist[index++] = loops[i][j].x;
			in.pointlist[index++] = loops[i][j].y;
		}
	}

	in.numberofsegments = bound.size() + lines.size() / 2 + loopSize;
	in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));

	index = 0;
	for (size_t i = 0, n = bound.size() - 1; i < n; ++i)
	{
		in.segmentlist[index++] = i;
		in.segmentlist[index++] = i + 1;
	}
	in.segmentlist[index++] = bound.size() - 1;
	in.segmentlist[index++] = 0;

	int lineIndex = bound.size();
	for (size_t i = 0, n = lines.size() / 2; i < n; ++i)
	{
		in.segmentlist[index++] = lineIndex++;
		in.segmentlist[index++] = lineIndex++;
	}

	int loopIndex = bound.size() + lines.size();
	for (size_t i = 0, n = loops.size(); i < n; ++i)
	{
		int start = loopIndex;
		for (size_t j = 0, m = loops[i].size() - 1; j < m; ++j)
		{
			in.segmentlist[index++] = loopIndex;
			in.segmentlist[index++] = loopIndex + 1;
			++loopIndex;
		}
		in.segmentlist[index++] = loopIndex;
		in.segmentlist[index++] = start;
		++loopIndex;
	}

	in.segmentmarkerlist = (int *) NULL;

	in.numberofholes = 0;
	in.numberofregions = 0;

	init(out);
	implement(in, out, type);
	finish(out, bound, result);
}

void Triangulation::strips(const std::vector<Vector>& triangulates, 
	std::vector<std::vector<Vector> >& strips)
{
	d2d::SGI::implement(triangulates, strips);
}

}