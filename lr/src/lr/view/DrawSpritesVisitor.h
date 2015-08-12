#ifndef _LR_DRAW_SPRITE_VISITOR_H_
#define _LR_DRAW_SPRITE_VISITOR_H_

#include <drag2d.h>

namespace lr
{

class DrawSpritesVisitor : public d2d::DrawSpritesVisitor
{
public:
	DrawSpritesVisitor(const d2d::Rect& screen_region, float cam_scale);

	void DrawSorted() const;

protected:
	virtual void DrawSprite(d2d::SpriteRenderer* rd, d2d::ISprite* spr) const;

private:
	mutable std::vector<d2d::ISprite*> m_need_sort;

}; // DrawSpritesVisitor

}

#endif // _LR_DRAW_SPRITE_VISITOR_H_