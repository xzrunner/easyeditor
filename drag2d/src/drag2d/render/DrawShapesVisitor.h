#ifndef _DRAG2D_DRAW_SHAPES_VISITOR_H_
#define _DRAG2D_DRAW_SHAPES_VISITOR_H_

#include "common/visitors.h"
#include "common/Color.h"
#include "common/Rect.h"

namespace d2d
{

class DrawShapesVisitor : public IVisitor
{
public:
	DrawShapesVisitor(const Rect& screen_region,
		const Colorf& color = Colorf(0, 0, 0));

	virtual void visit(Object* object, bool& bFetchNext);

private:
	Rect m_screen_region;

	Colorf m_color;

}; // DrawShapesVisitor

}

#endif // _DRAG2D_DRAW_SHAPES_VISITOR_H_