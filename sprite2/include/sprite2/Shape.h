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
	
	virtual bool IsContain(const sm::vec2& pos) const = 0;
	virtual bool IsIntersect(const sm::rect& rect) const = 0;

	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const = 0;
	
	const sm::rect& GetBounding() const {
		return m_bounding;
	}

protected:
	virtual void UpdateBounding() = 0;

protected:
	sm::rect m_bounding;

}; // Shape

}

#endif // _SPRITE2_SHAPE_H_