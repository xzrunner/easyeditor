#ifndef _EASYEDITOR_OBB_H_
#define _EASYEDITOR_OBB_H_

#include "BoundingBox.h"

namespace ee
{

class OBB : public BoundingBox
{
public:
	OBB();
	OBB(const OBB& obb);

	//
	// Cloneable
	//
	virtual OBB* Clone() const;

	virtual void InitFromRect(const sm::rect& rect);

	virtual bool IsValid() const;
	virtual void MakeEmpty();

	virtual void SetTransform(const sm::vec2& position, const sm::vec2& offset, float angle);
	virtual void SetMirror(bool xmirror, bool ymirror);

	virtual void Combine(const sm::vec2& pos);
	virtual void Combine(const sm::rect& rect);

	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsContain(const sm::rect& rect) const;
	virtual bool IsIntersect(const sm::rect& rect) const;

	virtual float Area() const;
	virtual float Width() const;
	virtual float Height() const;
	virtual sm::vec2 Center() const;

	virtual void GetBoundPos(std::vector<sm::vec2>& bound) const;

private:
	void TransToAABB();

private:
	sm::rect m_rect;
	sm::vec2 m_position;
	float m_angle;

}; // OBB

}

#include "OBB.inl"

#endif // _EASYEDITOR_OBB_H_