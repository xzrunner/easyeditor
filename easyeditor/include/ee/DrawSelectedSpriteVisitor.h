#ifndef _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_
#define _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_

#include "Visitor.h"
#include "Sprite.h"

#include <sprite2/Color.h>

namespace ee
{

class DrawSelectedSpriteVisitor : public Visitor<Sprite>
{
public:
	DrawSelectedSpriteVisitor(const s2::Color& color);

	virtual void Visit(Sprite* spr, bool& next);

private:
	s2::Color m_color;

}; // DrawSelectedSpriteVisitor

}

#endif // _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_