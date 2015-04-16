#ifndef _LIBSHAPE_SHAPE_TYPE_H_
#define _LIBSHAPE_SHAPE_TYPE_H_

namespace libshape
{

enum ShapeType
{
	e_unknown = 0,
	e_rect,
	e_circle,
	e_chain,
	e_polygon,
	e_bezier
}; // ShapeType

}

#endif // _LIBSHAPE_SHAPE_TYPE_H_
