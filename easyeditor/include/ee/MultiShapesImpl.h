#ifndef _EASYEDITOR_MULTI_SHAPES_IMPL_H_
#define _EASYEDITOR_MULTI_SHAPES_IMPL_H_

#include "DataTraverseType.h"
#include "Observer.h"
#include "Visitor.h"
#include "Shape.h"

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <wx/wx.h>

namespace ee
{

class StageCanvas;
class ShapeSelection;

class MultiShapesImpl : public Observer
{
public:
	MultiShapesImpl();
	virtual ~MultiShapesImpl();

	virtual void TraverseShapes(RefVisitor<Shape>& visitor,
		DataTraverseType type = DT_ALL) const = 0;

	ShapePtr QueryShapeByPos(const sm::vec2& pos) const;
	void QueryShapesByRect(const sm::rect& rect, std::vector<ShapePtr>& result) const;		

	ShapeSelection* GetShapeSelection() { return m_shape_selection; }
	void ClearSelectedShape();

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	class PointQueryVisitor : public RefVisitor<Shape>
	{
	public:
		PointQueryVisitor(const sm::vec2& pos, ShapePtr& ret);
		virtual void Visit(const ShapePtr& shape, bool& next);

	private:
		const sm::vec2& m_pos;
		ShapePtr& m_ret;

	}; // PointQueryVisitor

	class RectQueryVisitor : public RefVisitor<Shape>
	{
	public:
		RectQueryVisitor(const sm::rect& rect, std::vector<ShapePtr>& result);
		virtual void Visit(const ShapePtr& shape, bool& next);

	private:
		const sm::rect& m_rect;
		std::vector<ShapePtr>& m_result;

	}; // RectQueryVisitor

protected:
	ShapeSelection* m_shape_selection;

}; // MultiShapesImpl

}

#endif // _EASYEDITOR_MULTI_SHAPES_IMPL_H_