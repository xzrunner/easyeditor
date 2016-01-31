#ifndef _EASYEDITOR_OBB_H_
#define _EASYEDITOR_OBB_H_

#include "BoundingBox.h"
#include "Rect.h"

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

	virtual void InitFromRect(const Rect& rect);

	virtual bool IsValid() const;
	virtual void MakeInfinite();

	virtual void SetTransform(const Vector& position, const Vector& offset, float angle);
	virtual void SetMirror(bool xmirror, bool ymirror);

	virtual void Combine(const Vector& pos);
	virtual void Combine(const Rect& rect);

	virtual bool IsContain(const Vector& pos) const;
	virtual bool IsContain(const Rect& rect) const;
	virtual bool IsIntersect(const Rect& rect) const;

	virtual float Area() const;
	virtual float Width() const;
	virtual float Height() const;
	virtual Vector Center() const;

	virtual void GetBoundPos(std::vector<Vector>& bound) const;

private:
	void TransToAABB();

private:
	Rect m_rect;
	Vector m_position;
	float m_angle;

}; // OBB

}

#include "OBB.inl"

#endif // _EASYEDITOR_OBB_H_