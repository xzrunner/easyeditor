#ifndef _EASYMESH_EDIT_SHAPE_H_
#define _EASYMESH_EDIT_SHAPE_H_

#include "Shape.h"

namespace ee { class Visitor; class Shape; class Vector; }

namespace emesh
{

class EditShape : public Shape
{
public:
	EditShape() {}
	EditShape(const Shape& shape) 
		: Shape(shape) {}
	EditShape(const ee::Image& image) 
		: Shape(image) {}
	virtual ~EditShape() {}

	virtual void InsertNode(const ee::Vector& p) = 0;
	virtual void RemoveNode(const ee::Vector& p) = 0;
	virtual ee::Vector* FindNode(const ee::Vector& p) = 0;
	virtual void MoveNode(ee::Vector* src, const ee::Vector& dst) = 0;

	virtual void TraverseShape(ee::Visitor& visitor) const = 0;
	virtual bool RemoveShape(ee::Shape* shape) = 0;
	virtual bool InsertShape(ee::Shape* shape) = 0;
	virtual bool ClearShape() = 0;	
	
	virtual void Reset() = 0;
	virtual void Clear() = 0;

}; // EditShape

}

#endif // _EASYMESH_EDIT_SHAPE_H_