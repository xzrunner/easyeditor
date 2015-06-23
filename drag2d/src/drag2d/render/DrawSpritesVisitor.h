#ifndef _DRAG2D_DRAW_SPRITES_VISITOR_H_
#define _DRAG2D_DRAW_SPRITES_VISITOR_H_

#include "common/visitors.h"
#include "common/Rect.h"

namespace d2d
{

class DrawSpritesVisitor : public IVisitor
{
public:
	DrawSpritesVisitor(const Rect& screen_region) 
		: m_screen_region(screen_region) {}
	virtual void visit(Object* object, bool& bFetchNext);

private:
	Rect m_screen_region;

}; // DrawSpritesVisitor

}

#endif // _DRAG2D_DRAW_SPRITES_VISITOR_H_