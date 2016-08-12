#ifndef _EASYEDITOR_MULTI_SHAPES_IMPL_H_
#define _EASYEDITOR_MULTI_SHAPES_IMPL_H_

#include "DataTraverseType.h"
#include "Observer.h"
#include "Visitor.h"

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <wx/wx.h>

namespace ee
{

class StageCanvas;
class ShapeSelection;
class Shape;

class MultiShapesImpl : public Observer
{
public:
	MultiShapesImpl();
	virtual ~MultiShapesImpl();

	virtual void TraverseShapes(Visitor<Shape>& visitor, 
		DataTraverseType type = DT_ALL) const = 0;

	Shape* QueryShapeByPos(const sm::vec2& pos) const;
	void QueryShapesByRect(const sm::rect& rect, std::vector<Shape*>& result) const;		

	ShapeSelection* GetShapeSelection() { return m_shape_selection; }
	void ClearSelectedShape();

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	class PointQueryVisitor : public Visitor<Shape>
	{
	public:
		PointQueryVisitor(const sm::vec2& pos, Shape** pResult);
		virtual void Visit(Shape* shape, bool& next);

	private:
		const sm::vec2& m_pos;
		Shape** m_pResult;

	}; // PointQueryVisitor

	class RectQueryVisitor : public Visitor<Shape>
	{
	public:
		RectQueryVisitor(const sm::rect& rect, std::vector<Shape*>& result);
		virtual void Visit(Shape* shape, bool& next);

	private:
		const sm::rect& m_rect;
		std::vector<Shape*>& m_result;

	}; // RectQueryVisitor

protected:
	ShapeSelection* m_shape_selection;

}; // MultiShapesImpl

}

#endif // _EASYEDITOR_MULTI_SHAPES_IMPL_H_