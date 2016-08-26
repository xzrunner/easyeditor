#ifndef _SPRITE2_POLYLINE_SHAPE_H_
#define _SPRITE2_POLYLINE_SHAPE_H_

#include "Shape.h"

#include <vector>

namespace s2
{

class PolylineShape : public VIRTUAL_INHERITANCE Shape
{
public:
	PolylineShape();
	PolylineShape(const PolylineShape& polyline);
	PolylineShape(const std::vector<sm::vec2>& vertices, bool closed = false);
	
	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual PolylineShape* Clone() const;

	/**
	 *  @interface
	 *    Shape
	 */
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;
	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const;

	const std::vector<sm::vec2>& GetVertices() const { return m_vertices; }

protected:
	virtual void UpdateBounding();

protected:
	std::vector<sm::vec2> m_vertices;
	bool m_closed;

	VI_DUMMY_FUNC

}; // PolylineShape

}

#endif // _SPRITE2_POLYLINE_SHAPE_H_