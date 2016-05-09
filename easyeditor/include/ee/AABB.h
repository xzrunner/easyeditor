#ifndef _EASYEDITOR_AABB_H_
#define _EASYEDITOR_AABB_H_

#include "BoundingBox.h"
#include "Rect.h"

namespace ee
{

class AABB : public BoundingBox
{
public:
	AABB();
	AABB(const AABB& aabb);

	//
	// Cloneable
	//
	virtual AABB* Clone() const;

	virtual void InitFromRect(const Rect& rect);

	virtual bool IsValid() const;
	virtual void MakeInfinite();

	virtual void SetTransform(const sm::vec2& position, const sm::vec2& offset, float angle);
	virtual void SetMirror(bool xmirror, bool ymirror);

	virtual void Combine(const sm::vec2& pos);
	virtual void Combine(const Rect& rect);

	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsContain(const Rect& rect) const;
	virtual bool IsIntersect(const Rect& rect) const;
	
	virtual float Area() const;
	virtual float Width() const;
	virtual float Height() const;
	virtual sm::vec2 Center() const;

	virtual void GetBoundPos(std::vector<sm::vec2>& bound) const;

private:
	Rect m_rect;
	sm::vec2 m_position;

}; // AABB

}

#include "AABB.inl"

#endif // _EASYEDITOR_AABB_H_