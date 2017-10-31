#ifndef _EASYEDITOR_POLYGON_CLIPPER_H_
#define _EASYEDITOR_POLYGON_CLIPPER_H_

#include <SM_Vector.h>

#include <cu/cu_stl.h>

#include <clipper.hpp>

namespace ee
{

class PolygonClipper
{
public:
	static CU_VEC<CU_VEC<sm::vec2> >
		Intersection(const CU_VEC<CU_VEC<sm::vec2> >& subject, const CU_VEC<sm::vec2>& clip);

	static CU_VEC<CU_VEC<sm::vec2> >
		Union(const CU_VEC<CU_VEC<sm::vec2> >& subject, const CU_VEC<sm::vec2>& clip);

	static CU_VEC<CU_VEC<sm::vec2> >
		Difference(const CU_VEC<CU_VEC<sm::vec2> >& subject, const CU_VEC<sm::vec2>& clip);

	static CU_VEC<CU_VEC<sm::vec2> >
		Xor(const CU_VEC<CU_VEC<sm::vec2> >& subject, const CU_VEC<sm::vec2>& clip);

private:
	static ClipperLib620::Path CreatePath(const CU_VEC<sm::vec2>& points);
	static CU_VEC<sm::vec2> ParserPath(const ClipperLib620::Path& path);

	static CU_VEC<CU_VEC<sm::vec2> >
		Implement(const CU_VEC<CU_VEC<sm::vec2> >& subject, const CU_VEC<sm::vec2>& clip, ClipperLib620::ClipType type);

private:
	static const float SCALE;

}; // PolygonClipper

}

#endif // _EASYEDITOR_POLYGON_CLIPPER_H_