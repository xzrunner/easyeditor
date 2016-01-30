#ifndef _EASYEDITOR_POLYGON_CLIPPER_H_
#define _EASYEDITOR_POLYGON_CLIPPER_H_

#include "Vector.h"

#include <vector>

namespace ClipperLib620
{
	struct IntPoint;

	typedef std::vector< IntPoint > Path;
	typedef std::vector< Path > Paths;

	enum ClipType;
}

namespace ee
{

class PolygonClipper
{
public:
	static std::vector<std::vector<Vector> >
		Intersection(const std::vector<std::vector<Vector> >& subject, const std::vector<Vector>& clip);

	static std::vector<std::vector<Vector> >
		Union(const std::vector<std::vector<Vector> >& subject, const std::vector<Vector>& clip);

	static std::vector<std::vector<Vector> >
		Difference(const std::vector<std::vector<Vector> >& subject, const std::vector<Vector>& clip);

	static std::vector<std::vector<Vector> >
		Xor(const std::vector<std::vector<Vector> >& subject, const std::vector<Vector>& clip);

private:
	static ClipperLib620::Path CreatePath(const std::vector<Vector>& points);
	static std::vector<Vector> ParserPath(const ClipperLib620::Path& path);

	static std::vector<std::vector<Vector> >
		Implement(const std::vector<std::vector<Vector> >& subject, const std::vector<Vector>& clip, ClipperLib620::ClipType type);

private:
	static const float SCALE;

}; // PolygonClipper

}

#endif // _EASYEDITOR_POLYGON_CLIPPER_H_