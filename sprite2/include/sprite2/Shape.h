#ifndef _SPRITE2_SHAPE_H_
#define _SPRITE2_SHAPE_H_

#include "pre_defined.h"
#include "RenderColor.h"

#include <SM_Vector.h>
#include <SM_Rect.h>
#include <SM_Matrix.h>
#include <CU_RefCountObj.h>
#include <CU_Cloneable.h>

namespace s2
{

class Shape : public cu::RefCountObj, public cu::Cloneable
{
public:
	Shape() {}
	Shape(const Shape& shape) : m_bounding(shape.m_bounding) {}
	virtual ~Shape() {}

	virtual bool IsContain(const sm::vec2& pos) const = 0;
	virtual bool IsIntersect(const sm::rect& rect) const = 0;

	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const = 0;

	/**
	 *  @interface
	 *    Cloneable
	 *  @note
	 *    should after other virtual
	 */
	virtual Shape* Clone() const { return NULL; }

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