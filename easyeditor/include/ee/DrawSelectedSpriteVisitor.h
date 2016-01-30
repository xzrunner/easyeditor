#ifndef _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_
#define _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_

#include "Visitor.h"
#include "Color.h"

namespace ee
{

class DrawSelectedSpriteVisitor : public Visitor
{
public:
	DrawSelectedSpriteVisitor(const Colorf& color);

	virtual void Visit(Object* object, bool& next);

private:
	Colorf m_color;

}; // DrawSelectedSpriteVisitor

}

#endif // _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_