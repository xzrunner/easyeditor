#ifndef _EASYEDITOR_MULTI_SHAPES_IMPL_H_
#define _EASYEDITOR_MULTI_SHAPES_IMPL_H_

#include "DataTraverseType.h"
#include "Observer.h"
#include "Visitor.h"

#include <wx/wx.h>

namespace ee
{

class StageCanvas;
class ShapeSelection;
class Rect;
class Shape;
class Visitor;
class Vector;

class MultiShapesImpl : public Observer
{
public:
	MultiShapesImpl();
	virtual ~MultiShapesImpl();

	virtual void TraverseShapes(Visitor& visitor, 
		DataTraverseType type = DT_ALL) const = 0;

	Shape* QueryShapeByPos(const Vector& pos) const;
	void QueryShapesByRect(const Rect& rect, std::vector<Shape*>& result) const;		

	ShapeSelection* GetShapeSelection() { return m_shape_selection; }
	void ClearSelectedShape();

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	class PointQueryVisitor : public Visitor
	{
	public:
		PointQueryVisitor(const Vector& pos, Shape** pResult);
		virtual void Visit(Object* object, bool& next);

	private:
		const Vector& m_pos;
		Shape** m_pResult;

	}; // PointQueryVisitor

	class RectQueryVisitor : public Visitor
	{
	public:
		RectQueryVisitor(const Rect& rect, std::vector<Shape*>& result);
		virtual void Visit(Object* object, bool& next);

	private:
		const Rect& m_rect;
		std::vector<Shape*>& m_result;

	}; // RectQueryVisitor

protected:
	ShapeSelection* m_shape_selection;

}; // MultiShapesImpl

}

#endif // _EASYEDITOR_MULTI_SHAPES_IMPL_H_