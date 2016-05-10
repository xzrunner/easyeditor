#ifndef _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_
#define _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_

#include <sprite2/Color.h>

#include "Visitor.h"

namespace ee
{

class DrawSelectedSpriteVisitor : public Visitor
{
public:
	DrawSelectedSpriteVisitor(const s2::Color& color);

	virtual void Visit(Object* object, bool& next);

private:
	s2::Color m_color;

}; // DrawSelectedSpriteVisitor

}

#endif // _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_