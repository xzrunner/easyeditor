#ifndef _SPRITE2_CIRCLE_SHAPE_H_
#define _SPRITE2_CIRCLE_SHAPE_H_

#include "Shape.h"

namespace s2
{

class CircleShape : public VIRTUAL_INHERITANCE Shape
{
public:
	CircleShape() : m_radius(0) {}
	CircleShape(const CircleShape& circle);
	CircleShape(const sm::vec2& center, float radius);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual CircleShape* Clone() const;

	/**
	 *  @interface
	 *    Shape
	 */
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;
	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const;

protected:
	virtual void UpdateBounding();

protected:
	sm::vec2 m_center;
	float m_radius;

}; // CircleShape

}

#endif // _SPRITE2_CIRCLE_SHAPE_H_	