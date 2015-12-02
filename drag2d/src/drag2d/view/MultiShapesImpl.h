#ifndef _DRAG2D_MULTI_SHAPES_IMPL_H_
#define _DRAG2D_MULTI_SHAPES_IMPL_H_

#include "DataTraverseType.h"

#include "common/visitors.h"
#include "message/Observer.h"

#include <wx/wx.h>

namespace d2d
{

class IStageCanvas;
class ShapeSelection;
class Rect;
class IShape;

class MultiShapesImpl : public Observer
{
public:
	MultiShapesImpl();
	virtual ~MultiShapesImpl();

	virtual void TraverseShapes(IVisitor& visitor, 
		DataTraverseType type = DT_ALL) const = 0;

	IShape* QueryShapeByPos(const Vector& pos) const;
	void QueryShapesByRect(const Rect& rect, std::vector<IShape*>& result) const;		

	ShapeSelection* GetShapeSelection() { return m_shape_selection; }
	void ClearSelectedShape();

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	class PointQueryVisitor : public IVisitor
	{
	public:
		PointQueryVisitor(const Vector& pos, IShape** pResult);
		virtual void Visit(Object* object, bool& bFetchNext);

	private:
		const Vector& m_pos;
		IShape** m_pResult;

	}; // PointQueryVisitor

	class RectQueryVisitor : public IVisitor
	{
	public:
		RectQueryVisitor(const Rect& rect, std::vector<IShape*>& result);
		virtual void Visit(Object* object, bool& bFetchNext);

	private:
		const Rect& m_rect;
		std::vector<IShape*>& m_result;

	}; // RectQueryVisitor

protected:
	ShapeSelection* m_shape_selection;

}; // MultiShapesImpl

}

#endif // _DRAG2D_MULTI_SHAPES_IMPL_H_