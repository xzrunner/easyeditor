#ifndef _SPRITE2_POINT_SHAPE_H_
#define _SPRITE2_POINT_SHAPE_H_

#include "Shape.h"

namespace s2
{

class PointShape : public Shape
{
public:
	PointShape(const sm::vec2& pos);
	
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;

	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const;

private:
	sm::vec2 m_pos;

}; // PointShape

}

#endif // _SPRITE2_POINT_SHAPE_H_