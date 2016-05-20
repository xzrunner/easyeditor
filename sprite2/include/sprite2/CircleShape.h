#ifndef _SPRITE2_CIRCLE_SHAPE_H_
#define _SPRITE2_CIRCLE_SHAPE_H_

#include "Shape.h"

namespace s2
{

class CircleShape : public Shape
{
public:
	CircleShape() : m_radius(0) {}
	CircleShape(const sm::vec2& center, float radius);

	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;

	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const;

	const sm::vec2& GetCenter() const { return m_center; }
	void SetCenter(const sm::vec2& center);

	float GetRadius() const { return m_radius; }
	void SetRaidius(float radius);

private:
	sm::vec2 m_center;
	float m_radius;

}; // CircleShape

}

#endif // _SPRITE2_CIRCLE_SHAPE_H_	