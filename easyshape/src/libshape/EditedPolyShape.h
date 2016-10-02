#ifndef _EASYSHAPE_EDITED_POLY_SHAPE_H_
#define _EASYSHAPE_EDITED_POLY_SHAPE_H_

#include <ee/Shape.h>

#include <SM_Vector.h>

#include <vector>

namespace eshape
{

class EditedPolyShape : public ee::Shape
{
public:
	virtual void AddVertex(int index, const sm::vec2& pos) = 0;
	virtual void RemoveVertex(const sm::vec2& pos) = 0;
	virtual void ChangeVertex(const sm::vec2& from, const sm::vec2& to) = 0;
	virtual void SetVertices(const std::vector<sm::vec2>& vertices) = 0;
	virtual const std::vector<sm::vec2>& GetVertices() const  = 0;
	virtual bool IsClosed() const = 0;

}; // EditedPolyShape

}

#endif // _EASYSHAPE_EDITED_POLY_SHAPE_H_