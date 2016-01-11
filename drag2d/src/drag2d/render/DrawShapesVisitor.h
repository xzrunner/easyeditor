#ifndef _DRAG2D_DRAW_SHAPES_VISITOR_H_
#define _DRAG2D_DRAW_SHAPES_VISITOR_H_

#include "common/visitors.h"
#include "common/Color.h"
#include "common/Rect.h"
#include "dataset/ColorTrans.h"

namespace d2d
{

class DrawShapesVisitor : public IVisitor
{
public:
	DrawShapesVisitor(const Rect& screen_region);

	virtual void Visit(Object* object, bool& bFetchNext);

private:
	Rect m_screen_region;

	ColorTrans m_ct;

}; // DrawShapesVisitor

}

#endif // _DRAG2D_DRAW_SHAPES_VISITOR_H_