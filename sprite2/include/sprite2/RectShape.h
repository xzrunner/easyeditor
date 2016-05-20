#ifndef _SPRITE2_RECT_SHAPE_H_
#define _SPRITE2_RECT_SHAPE_H_

#include "Shape.h"

namespace s2
{

class RectShape : public Shape
{
public:
	RectShape(const sm::rect& r);
	
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;

	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const;

	const sm::rect& GetRect() const { return m_region; }
	void SetRect(const sm::rect& r) { m_region = r; }

}; // RectShape

}

#endif // _SPRITE2_RECT_SHAPE_H_