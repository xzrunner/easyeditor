#ifndef _SPRITE2_POLYGON_SHAPE_H_
#define _SPRITE2_POLYGON_SHAPE_H_

#include "PolylineShape.h"

namespace s2
{

class Polygon;

class PolygonShape : public PolylineShape
{
public:
	PolygonShape();
	PolygonShape(const PolygonShape& poly);
	PolygonShape& operator = (const PolygonShape& poly);
	PolygonShape(const std::vector<sm::vec2>& vertices);
	virtual ~PolygonShape();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual PolygonShape* Clone() const;	

	/**
	 *  @interface
	 *    Shape
	 */
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;
	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const;

	void SetPolygon(Polygon* poly);
	const Polygon* GetPolygon() const { return m_poly; }

protected:
	Polygon* m_poly;

}; // PolygonShape

}

#endif // _SPRITE2_POLYGON_SHAPE_H_