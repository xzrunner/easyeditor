#ifndef _EASYEDITOR_BOUNDING_BOX_H_
#define _EASYEDITOR_BOUNDING_BOX_H_

#include "Cloneable.h"

#include <vector>

namespace ee
{

class Rect;
class Vector;

class BoundingBox : public Cloneable
{
public:
	//
	// Cloneable
	//
	virtual BoundingBox* Clone() const = 0;

	virtual void InitFromRect(const Rect& rect) = 0;

	virtual bool IsValid() const = 0;
	virtual void MakeInfinite() = 0;

	virtual void SetTransform(const Vector& position, const Vector& offset, float angle) = 0;
	virtual void SetMirror(bool xmirror, bool ymirror) = 0;

	virtual void Combine(const Vector& pos) = 0;
	virtual void Combine(const Rect& rect) = 0;

	virtual bool IsContain(const Vector& pos) const = 0;
	virtual bool IsContain(const Rect& rect) const = 0;
	virtual bool IsIntersect(const Rect& rect) const = 0;

	virtual float Area() const = 0;
	virtual float Width() const = 0;
	virtual float Height() const = 0;
	virtual Vector Center() const = 0;

	virtual void GetBoundPos(std::vector<Vector>& bound) const = 0;

}; // BoundingBox

}

#endif // _EASYEDITOR_BOUNDING_BOX_H_