#include "ShapeType.h"

namespace eshape
{

ShapeType get_shape_type(const std::string& desc)
{
	if (desc == "point") {
		return ST_POINT;
	} else if (desc == "bezier") {
		return ST_BEZIER;
	} else if (desc == "chain") {
		return ST_CHAIN;
	} else if (desc == "circle") {
		return ST_CIRCLE;
	} else if (desc == "complex-polygon") {
		return ST_COMPLEX_POLYGON;
	} else if (desc == "cosine-curve") {
		return ST_COSINE_CURVE;
	} else if (desc == "polygon") {
		return ST_POLYGON;
	} else if (desc == "rect") {
		return ST_RECT;
	}

	return ST_UNKNOWN;
}

}