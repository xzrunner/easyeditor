#ifndef _EASYEDITOR_POLYGON_CLIPPER_H_
#define _EASYEDITOR_POLYGON_CLIPPER_H_

#include <SM_Vector.h>

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
	static std::vector<std::vector<sm::vec2> >
		Intersection(const std::vector<std::vector<sm::vec2> >& subject, const std::vector<sm::vec2>& clip);

	static std::vector<std::vector<sm::vec2> >
		Union(const std::vector<std::vector<sm::vec2> >& subject, const std::vector<sm::vec2>& clip);

	static std::vector<std::vector<sm::vec2> >
		Difference(const std::vector<std::vector<sm::vec2> >& subject, const std::vector<sm::vec2>& clip);

	static std::vector<std::vector<sm::vec2> >
		Xor(const std::vector<std::vector<sm::vec2> >& subject, const std::vector<sm::vec2>& clip);

private:
	static ClipperLib620::Path CreatePath(const std::vector<sm::vec2>& points);
	static std::vector<sm::vec2> ParserPath(const ClipperLib620::Path& path);

	static std::vector<std::vector<sm::vec2> >
		Implement(const std::vector<std::vector<sm::vec2> >& subject, const std::vector<sm::vec2>& clip, ClipperLib620::ClipType type);

private:
	static const float SCALE;

}; // PolygonClipper

}

#endif // _EASYEDITOR_POLYGON_CLIPPER_H_