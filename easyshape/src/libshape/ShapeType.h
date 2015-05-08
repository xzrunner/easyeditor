#ifndef _LIBSHAPE_SHAPE_TYPE_H_
#define _LIBSHAPE_SHAPE_TYPE_H_

#include <string>

namespace libshape
{

enum ShapeType
{
	ST_UNKNOWN = 0,
	ST_RECT,
	ST_CIRCLE,
	ST_CHAIN,
	ST_COSINE_CURVE,
	ST_POLYGON,
	ST_COMPLEX_POLYGON,
	ST_BEZIER
}; // ShapeType

ShapeType get_shape_type(const std::string& desc);

}

#endif // _LIBSHAPE_SHAPE_TYPE_H_
