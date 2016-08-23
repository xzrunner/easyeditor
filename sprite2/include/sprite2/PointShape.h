#ifndef _SPRITE2_POINT_SHAPE_H_
#define _SPRITE2_POINT_SHAPE_H_

#include "Shape.h"

namespace s2
{

class PointShape : public VIRTUAL_INHERITANCE Shape
{
public:
	PointShape() {}
	PointShape(const PointShape& point);
	PointShape(const sm::vec2& pos) 
		: m_pos(pos) 
	{
		UpdateBounding();
	}

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual PointShape* Clone() const;	

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

	VI_DUMMY_FUNC

protected:
	sm::vec2 m_pos;

}; // PointShape

}

#endif // _SPRITE2_POINT_SHAPE_H_