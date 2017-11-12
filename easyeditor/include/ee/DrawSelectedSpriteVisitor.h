#ifndef _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_
#define _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_

#include "Visitor.h"
#include "Sprite.h"

#include <sprite2/Color.h>

namespace ee
{

class DrawSelectedSpriteVisitor : public RefVisitor<Sprite>
{
public:
	DrawSelectedSpriteVisitor(const s2::Color& color);

	virtual void Visit(const SprPtr& spr, bool& next) override;

private:
	s2::Color m_color;

}; // DrawSelectedSpriteVisitor

}

#endif // _EASYEDITOR_DRAW_SELECTED_SPRITE_VISITOR_H_