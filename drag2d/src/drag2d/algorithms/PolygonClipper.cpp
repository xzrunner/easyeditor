#include "PolygonClipper.h"

#include <clipper.hpp>

namespace d2d
{

const float PolygonClipper::SCALE = 10.0f;

std::vector<std::vector<d2d::Vector> > 
PolygonClipper::Intersection(const std::vector<std::vector<d2d::Vector> >& subject, 
							 const std::vector<d2d::Vector>& clip)
{
	return Implement(subject, clip, ClipperLib::ctIntersection);
}

std::vector<std::vector<d2d::Vector> > 
PolygonClipper::Union(const std::vector<std::vector<d2d::Vector> >& subject, 
					  const std::vector<d2d::Vector>& clip)
{
	return Implement(subject, clip, ClipperLib::ctUnion);
}

std::vector<std::vector<d2d::Vector> > 
PolygonClipper::Difference(const std::vector<std::vector<d2d::Vector> >& subject, 
						   const std::vector<d2d::Vector>& clip)
{
	return Implement(subject, clip, ClipperLib::ctDifference);
}

std::vector<std::vector<d2d::Vector> > 
PolygonClipper::Xor(const std::vector<std::vector<d2d::Vector> >& subject, 
					const std::vector<d2d::Vector>& clip)
{
	return Implement(subject, clip, ClipperLib::ctXor);
}

ClipperLib::Path 
PolygonClipper::CreatePath(const std::vector<d2d::Vector>& points)
{
	ClipperLib::Path path;
	path.resize(points.size());
	for (int i = 0, n = points.size(); i < n; ++i) {
		path[i].X = (int)(points[i].x * SCALE);
		path[i].Y = (int)(points[i].y * SCALE);
	}
	return path;
}

std::vector<d2d::Vector> 
PolygonClipper::ParserPath(const ClipperLib::Path& path)
{
	std::vector<d2d::Vector> points;
	points.resize(path.size());
	for (int i = 0, n = path.size(); i < n; ++i) {
		points[i].x = path[i].X / SCALE;
		points[i].y = path[i].Y / SCALE;
	}
	return points;
}

std::vector<std::vector<d2d::Vector> >
PolygonClipper::Implement(const std::vector<std::vector<d2d::Vector> >& subject, 
						  const std::vector<d2d::Vector>& clip, 
						  ClipperLib::ClipType type)
{
	ClipperLib::Paths sub_paths, clp_paths;
	ClipperLib::Paths sol;

	sub_paths.resize(subject.size());
	for (int i = 0, n = subject.size(); i < n; ++i) {
		sub_paths[i] = CreatePath(subject[i]);
	}

	clp_paths.push_back(CreatePath(clip));

	ClipperLib::Clipper clipper;

	clipper.AddPaths(sub_paths, ClipperLib::ptSubject, true);
	clipper.AddPaths(clp_paths, ClipperLib::ptClip, true);

	clipper.Execute(type, sol, ClipperLib::pftEvenOdd);	

	std::vector<std::vector<d2d::Vector> > result;
	result.resize(sol.size());
	for (int i = 0, n = sol.size(); i < n; ++i) {
		result[i] = ParserPath(sol[i]);
	}

	return result;
}

}