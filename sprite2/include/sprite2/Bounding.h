#ifndef _SPRITE2_BOUNDING_H_
#define _SPRITE2_BOUNDING_H_

#include <SM_Vector.h>
#include <SM_Rect.h>

namespace s2
{

class Bounding
{
public:	

	virtual bool IsContain(const sm::vec2& pos) const = 0;
	virtual bool IsContain(const sm::rect& rect) const = 0;
	virtual bool IsIntersect(const sm::rect& rect) const = 0;

	virtual void CombineTo(sm::rect& r) const = 0;

	virtual void Build(const sm::rect& r, const sm::vec2& pos, float angle,
		const sm::vec2& scale, const sm::vec2& shear, const sm::vec2& offset) = 0;

}; // Bounding

}

#endif // _SPRITE2_BOUNDING_H_