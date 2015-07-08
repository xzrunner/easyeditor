#ifndef _DRAG2D_MULTI_SHAPES_IMPL_H_
#define _DRAG2D_MULTI_SHAPES_IMPL_H_

#include "IShapeViewPanel.h"
#include "DataTraverseType.h"

#include "dataset/ObjSelectionSet.h"

#include <wx/wx.h>

namespace d2d
{

class MultiShapesImpl : public IShapeViewPanel
{
public:
	MultiShapesImpl(wxWindow* wnd);
	virtual ~MultiShapesImpl();

	//
	//	interface IShapeViewPanel
	//
	virtual void SelectShape(IShape* shape);
	virtual void SelectMultiShapes(ShapeSelection* selection);

	virtual bool InsertShape(IShape* shape) = 0;
	virtual bool RemoveShape(IShape* shape) = 0;
	virtual bool ClearAllShapes() = 0;

	virtual void TraverseShapes(IVisitor& visitor, 
		DataTraverseType type = DT_ALL) const = 0;

	IShape* QueryShapeByPos(const Vector& pos) const;
	void QueryShapesByRect(const Rect& rect, std::vector<IShape*>& result) const;		

	ShapeSelection* GetShapeSelection() { return m_shape_selection; }
	void ClearShapeSelection();

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

	class RemoveSelectionVisitor : public IVisitor
	{
	public:
		RemoveSelectionVisitor(MultiShapesImpl* shapesImpl);
		virtual void Visit(Object* object, bool& bFetchNext);

	private:
		MultiShapesImpl* m_shapesImpl;

	}; // RemoveSelectionVisitor

protected:
	ShapeSelection* m_shape_selection;

private:
	wxWindow* m_wnd;

}; // MultiShapesImpl

}

#endif // _DRAG2D_MULTI_SHAPES_IMPL_H_