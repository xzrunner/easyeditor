#ifndef _EASYEDITOR_DRAW_SPRITES_VISITOR_H_
#define _EASYEDITOR_DRAW_SPRITES_VISITOR_H_

#include "Visitor.h"
#include "Rect.h"

namespace ee
{

class SpriteRenderer;
class Sprite;

class DrawSpritesVisitor : public Visitor
{
public:
	DrawSpritesVisitor(const Rect& screen_region, float cam_scale) 
		: m_screen_region(screen_region), m_cam_scale(cam_scale) {}
	virtual void Visit(Object* object, bool& next);

protected:
	virtual void DrawSprite(SpriteRenderer* rd, Sprite* spr) const;

private:
	Rect m_screen_region;

	float m_cam_scale;

}; // DrawSpritesVisitor

}

#endif // _EASYEDITOR_DRAW_SPRITES_VISITOR_H_