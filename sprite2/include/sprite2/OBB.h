#ifndef _SPRITE2_OBB_H_
#define _SPRITE2_OBB_H_

#include "BoundingBox.h"

namespace s2
{

class OBB : public BoundingBox
{ 
public:
	OBB();

	/**
	 *  @interface
	 *    cu::Cloneable
	 */
	virtual OBB* Clone() const { return new OBB(*this); }	

	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsContain(const sm::rect& rect) const;
	virtual bool IsIntersect(const sm::rect& rect) const;

	virtual void CombineTo(sm::rect& r) const;

	virtual void Build(const sm::rect& r, const sm::vec2& pos, float angle,
		const sm::vec2& scale, const sm::vec2& shear, const sm::vec2& offset);
	virtual void SetTransform(const sm::vec2& position, const sm::vec2& offset, float angle);

private:
	sm::rect m_rect;
	sm::vec2 m_position;
	float    m_angle;

}; // OBB

}

#include "OBB.inl"

#endif // _SPRITE2_OBB_H_