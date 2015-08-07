#ifndef _LR_SHAPES_UD_H_
#define _LR_SHAPES_UD_H_

namespace lr
{

struct ShapesUD
{
	Layer* layer;
	std::vector<d2d::IShape*> shapes;
}; // ShapesUD

}

#endif // _LR_SHAPES_UD_H_