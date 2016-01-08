#ifndef _DRAG2D_DRAW_SELECTED_SHAPE_VISITOR_H_
#define _DRAG2D_DRAW_SELECTED_SHAPE_VISITOR_H_

#include "common/visitors.h"

namespace d2d
{

class IShape;
class Matrix;

class DrawSelectedShapeVisitor : public IVisitor
{
public:
	virtual void Visit(Object* object, bool& bFetchNext);

}; // DrawSelectedShapeVisitor

}

#endif // _DRAG2D_DRAW_SELECTED_SHAPE_VISITOR_H_