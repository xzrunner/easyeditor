#pragma once

#include <vector>
#include <wx/wx.h>

#include "interfaces.h"
#include "DataTraverseType.h"

#include "dataset/ObjSelectionSet.h"

namespace d2d
{
	class MultiShapesImpl
	{
	public:
		MultiShapesImpl(wxWindow* wnd);
		virtual ~MultiShapesImpl();

		virtual void traverseShapes(IVisitor& visitor, 
			DataTraverseType type = DT_ALL) const = 0;
		virtual void removeShape(IShape* shape) = 0;
		virtual void insertShape(IShape* shape) = 0;
		virtual void clearShapes() = 0;

		IShape* queryShapeByPos(const Vector& pos) const;
		void queryShapesByRect(const Rect& rect, std::vector<IShape*>& result) const;		

		ShapeSelection* getShapeSelection() { return m_shapeSelection; }
		void removeShapeSelection();

	protected:
		void refresh();

	private:
		class PointQueryVisitor : public IVisitor
		{
		public:
			PointQueryVisitor(const Vector& pos, IShape** pResult);
			virtual void visit(Object* object, bool& bFetchNext);

		private:
			const Vector& m_pos;
			IShape** m_pResult;

		}; // PointQueryVisitor

		class RectQueryVisitor : public IVisitor
		{
		public:
			RectQueryVisitor(const Rect& rect, std::vector<IShape*>& result);
			virtual void visit(Object* object, bool& bFetchNext);

		private:
			const Rect& m_rect;
			std::vector<IShape*>& m_result;

		}; // RectQueryVisitor

		class RemoveSelectionVisitor : public IVisitor
		{
		public:
			RemoveSelectionVisitor(MultiShapesImpl* shapesImpl);
			virtual void visit(Object* object, bool& bFetchNext);

		private:
			MultiShapesImpl* m_shapesImpl;

		}; // RemoveSelectionVisitor

	protected:
		ShapeSelection* m_shapeSelection;

	private:
		wxWindow* m_wnd;

	}; // MultiShapesImpl
}

