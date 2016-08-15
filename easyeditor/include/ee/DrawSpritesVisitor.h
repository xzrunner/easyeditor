#ifndef _EASYEDITOR_DRAW_SPRITES_VISITOR_H_
#define _EASYEDITOR_DRAW_SPRITES_VISITOR_H_

#include "Visitor.h"

#include <SM_Rect.h>

namespace ee
{

class SpriteRenderer;
class Sprite;

class DrawSpritesVisitor : public Visitor<Sprite>
{
public:
	DrawSpritesVisitor(const sm::rect& screen_region, float cam_scale) 
		: m_screen_region(screen_region), m_cam_scale(cam_scale) {}
	virtual void Visit(Sprite* spr, bool& next);

protected:
	virtual void DrawSprite(Sprite* spr) const;

private:
	sm::rect m_screen_region;

	float m_cam_scale;

}; // DrawSpritesVisitor

}

#endif // _EASYEDITOR_DRAW_SPRITES_VISITOR_H_