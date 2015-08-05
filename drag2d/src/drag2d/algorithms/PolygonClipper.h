#ifndef _DRAG2D_POLYGON_CLIPPER_H_
#define _DRAG2D_POLYGON_CLIPPER_H_

#include "common/Vector.h"

#include <vector>

namespace ClipperLib620
{
	struct IntPoint;

	typedef std::vector< IntPoint > Path;
	typedef std::vector< Path > Paths;

	enum ClipType;
}

namespace d2d
{

class PolygonClipper
{
public:
	static std::vector<std::vector<d2d::Vector> >
		Intersection(const std::vector<std::vector<d2d::Vector> >& subject, const std::vector<d2d::Vector>& clip);

	static std::vector<std::vector<d2d::Vector> >
		Union(const std::vector<std::vector<d2d::Vector> >& subject, const std::vector<d2d::Vector>& clip);

	static std::vector<std::vector<d2d::Vector> >
		Difference(const std::vector<std::vector<d2d::Vector> >& subject, const std::vector<d2d::Vector>& clip);

	static std::vector<std::vector<d2d::Vector> >
		Xor(const std::vector<std::vector<d2d::Vector> >& subject, const std::vector<d2d::Vector>& clip);

private:
	static ClipperLib620::Path CreatePath(const std::vector<d2d::Vector>& points);
	static std::vector<d2d::Vector> ParserPath(const ClipperLib620::Path& path);

	static std::vector<std::vector<d2d::Vector> >
		Implement(const std::vector<std::vector<d2d::Vector> >& subject, const std::vector<d2d::Vector>& clip, ClipperLib620::ClipType type);

private:
	static const float SCALE;

}; // PolygonClipper

}

#endif // _DRAG2D_POLYGON_CLIPPER_H_