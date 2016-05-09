#include "PolygonClipper.h"

#include <clipper.hpp>

namespace ee
{

const float PolygonClipper::SCALE = 10.0f;

std::vector<std::vector<sm::vec2> > 
PolygonClipper::Intersection(const std::vector<std::vector<sm::vec2> >& subject, 
							 const std::vector<sm::vec2>& clip)
{
	return Implement(subject, clip, ClipperLib620::ctIntersection);
}

std::vector<std::vector<sm::vec2> > 
PolygonClipper::Union(const std::vector<std::vector<sm::vec2> >& subject, 
					  const std::vector<sm::vec2>& clip)
{
	return Implement(subject, clip, ClipperLib620::ctUnion);
}

std::vector<std::vector<sm::vec2> > 
PolygonClipper::Difference(const std::vector<std::vector<sm::vec2> >& subject, 
						   const std::vector<sm::vec2>& clip)
{
	return Implement(subject, clip, ClipperLib620::ctDifference);
}

std::vector<std::vector<sm::vec2> > 
PolygonClipper::Xor(const std::vector<std::vector<sm::vec2> >& subject, 
					const std::vector<sm::vec2>& clip)
{
	return Implement(subject, clip, ClipperLib620::ctXor);
}

ClipperLib620::Path 
PolygonClipper::CreatePath(const std::vector<sm::vec2>& points)
{
	ClipperLib620::Path path;
	path.resize(points.size());
	for (int i = 0, n = points.size(); i < n; ++i) {
		path[i].X = (int)(points[i].x * SCALE);
		path[i].Y = (int)(points[i].y * SCALE);
	}
	return path;
}

std::vector<sm::vec2> 
PolygonClipper::ParserPath(const ClipperLib620::Path& path)
{
	std::vector<sm::vec2> points;
	points.resize(path.size());
	for (int i = 0, n = path.size(); i < n; ++i) {
		points[i].x = path[i].X / SCALE;
		points[i].y = path[i].Y / SCALE;
	}
	return points;
}

std::vector<std::vector<sm::vec2> >
PolygonClipper::Implement(const std::vector<std::vector<sm::vec2> >& subject, 
						  const std::vector<sm::vec2>& clip, 
						  ClipperLib620::ClipType type)
{
	ClipperLib620::Paths sub_paths, clp_paths;
	ClipperLib620::Paths sol;

	sub_paths.resize(subject.size());
	for (int i = 0, n = subject.size(); i < n; ++i) {
		sub_paths[i] = CreatePath(subject[i]);
	}

	clp_paths.push_back(CreatePath(clip));

	ClipperLib620::Clipper clipper;

	clipper.AddPaths(sub_paths, ClipperLib620::ptSubject, true);
	clipper.AddPaths(clp_paths, ClipperLib620::ptClip, true);

	clipper.Execute(type, sol, ClipperLib620::pftEvenOdd);	

	std::vector<std::vector<sm::vec2> > result;
	result.resize(sol.size());
	for (int i = 0, n = sol.size(); i < n; ++i) {
		result[i] = ParserPath(sol[i]);
	}

	return result;
}

}