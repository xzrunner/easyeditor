#pragma once

#include <SM_Vector.h>
#include <SM_Rect.h>
#include <cu/cu_stl.h>

namespace pt2
{

class BoundingBox
{
public:	
	virtual ~BoundingBox() {}

	virtual BoundingBox* Clone() const = 0;

	virtual size_t Size() const = 0;

	virtual bool IsContain(const sm::vec2& pos) const = 0;
	virtual bool IsContain(const sm::rect& rect) const = 0;
	virtual bool IsIntersect(const sm::rect& rect) const = 0;

	virtual void CombineTo(sm::rect& r) const = 0;

	virtual void Build(const sm::rect& r, const sm::vec2& pos, float angle,
		const sm::vec2& scale, const sm::vec2& shear, const sm::vec2& offset) = 0;
	virtual void SetTransform(const sm::vec2& position, const sm::vec2& offset, float angle) = 0;

	virtual void GetBoundPos(CU_VEC<sm::vec2>& bound) const = 0;

	sm::rect GetSize() const {
		sm::rect r;
		CombineTo(r);
		return r;
	}

	static void Deleter(BoundingBox* bb);

}; // BoundingBox

}
