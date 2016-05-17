#ifndef _EASYEDITOR_BOUNDING_BOX_H_
#define _EASYEDITOR_BOUNDING_BOX_H_

#include "Cloneable.h"

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <vector>

namespace ee
{

class BoundingBox : public Cloneable
{
public:
	//
	// Cloneable
	//
	virtual BoundingBox* Clone() const = 0;

	virtual void InitFromRect(const sm::rect& rect) = 0;

	virtual bool IsValid() const = 0;
	virtual void MakeEmpty() = 0;

	virtual void SetTransform(const sm::vec2& position, const sm::vec2& offset, float angle) = 0;
	virtual void SetMirror(bool xmirror, bool ymirror) = 0;

	virtual void Combine(const sm::vec2& pos) = 0;
	virtual void Combine(const sm::rect& rect) = 0;

	virtual bool IsContain(const sm::vec2& pos) const = 0;
	virtual bool IsContain(const sm::rect& rect) const = 0;
	virtual bool IsIntersect(const sm::rect& rect) const = 0;

	virtual float Area() const = 0;
	virtual float Width() const = 0;
	virtual float Height() const = 0;
	virtual sm::vec2 Center() const = 0;

	virtual void GetBoundPos(std::vector<sm::vec2>& bound) const = 0;

}; // BoundingBox

}

#endif // _EASYEDITOR_BOUNDING_BOX_H_