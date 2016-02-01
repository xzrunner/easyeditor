#ifndef _EASYSHAPE_SHAPE_TYPE_H_
#define _EASYSHAPE_SHAPE_TYPE_H_

#include <string>

namespace eshape
{

enum ShapeType
{
	ST_UNKNOWN = 0,
	ST_POINT,
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

#endif // _EASYSHAPE_SHAPE_TYPE_H_
