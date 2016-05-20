#ifndef _SPRITE2_SHAPE_H_
#define _SPRITE2_SHAPE_H_

#include <SM_Vector.h>
#include <SM_Rect.h>
#include <SM_Matrix.h>

#include "RenderColor.h"

namespace s2
{

class Shape
{
public:
	Shape() {}
	Shape(const sm::rect& r) : m_region(r) {}
	
	virtual bool IsContain(const sm::vec2& pos) const = 0;
	virtual bool IsIntersect(const sm::rect& rect) const = 0;

	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const = 0;
	
	const sm::rect& GetRegion() const {
		return m_region;
	}

protected:
	sm::rect m_region;

}; // Shape

}

#endif // _SPRITE2_SHAPE_H_