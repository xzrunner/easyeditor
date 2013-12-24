#pragma once

#include "view/MultiShapesImpl.h"
#include "component/NodeCaptureCMPT.h"
#include "dataset/ChainShape.h"
#include "NodeCapture.h"

namespace d2d
{
	class EditPanel;
	class MultiShapesImpl;
	class ChainShape;
	class SelectShapesOP;
	class PropertySettingPanel;

	template <typename TBase, typename TSelected>
	class EditPolylineOP : public TBase
	{
	public:
		EditPolylineOP(EditPanel* editPanel, 
			MultiShapesImpl* shapesImpl, 
			PropertySettingPanel* propertyPanel,
			NodeCaptureCMPT<EditPolylineOP>* cmpt);
		virtual ~EditPolylineOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseMove(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		void drawCaptured(const NodeAddr& captured) const;

		void checkActiveShape(const NodeAddr& captured);

	private:
		class InterruptChainVisitor : public IVisitor
		{
		public:
			InterruptChainVisitor(const Vector& pos, int tol);

			virtual void visit(Object* object, bool& bFetchNext);

			ChainShape* getInterruptedChain() { return m_chain; }

		private:
			const Vector& m_pos;
			int m_tol;
			ChainShape* m_chain;

		}; // InterruptChainVisitor

		class NearestNodeVisitor : public IVisitor
		{
		public:
			NearestNodeVisitor(const Vector& pos, int tol);

			virtual void visit(Object* object, bool& bFetchNext);

			const Vector& getNearestNode() const {
				return m_nearest;
			}

		private:
			Vector m_pos;
			int m_tol;

			float m_dis;
			Vector m_nearest;

		}; // NearestNodeVisitor

	private:
		static const int DRAG_SELECT_TOL = 5;

	private:
		MultiShapesImpl* m_shapesImpl;

		PropertySettingPanel* m_propertyPanel;

		NodeCaptureCMPT<EditPolylineOP>* m_cmpt;
//		NodeAddr m_captured;

	protected:
		NodeAddr m_capturedEditable, m_captureSelectable;

	protected:
		TSelected* m_selectOP;
	private:
		Vector m_lastLeftDownPos;
		bool m_bSelectOpen;

	}; // EditPolylineOP
}

#include "EditPolylineOP.cpp"

