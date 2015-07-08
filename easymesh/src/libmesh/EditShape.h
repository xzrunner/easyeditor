#ifndef _EASYMESH_EDIT_SHAPE_H_
#define _EASYMESH_EDIT_SHAPE_H_

#include "Shape.h"

namespace emesh
{

class EditShape : public Shape
{
public:
	EditShape() {}
	EditShape(const Shape& shape) 
		: Shape(shape) {}
	EditShape(const d2d::Image& image) 
		: Shape(image) {}
	virtual ~EditShape() {}

	virtual void InsertNode(const d2d::Vector& p) = 0;
	virtual void RemoveNode(const d2d::Vector& p) = 0;
	virtual d2d::Vector* FindNode(const d2d::Vector& p) = 0;
	virtual void MoveNode(d2d::Vector* src, const d2d::Vector& dst) = 0;

	virtual void TraverseShape(d2d::IVisitor& visitor) const = 0;
	virtual bool RemoveShape(d2d::IShape* shape) = 0;
	virtual bool InsertShape(d2d::IShape* shape) = 0;
	virtual bool ClearShape() = 0;	
	
	virtual void Reset() = 0;
	virtual void Clear() = 0;

}; // EditShape

}

#endif // _EASYMESH_EDIT_SHAPE_H_