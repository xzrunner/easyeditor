#ifndef _LIBSHAPE_EDIT_POLYLINE_IMPL_H_
#define _LIBSHAPE_EDIT_POLYLINE_IMPL_H_

#include <drag2d.h>

#include "NodeCapture.h"

namespace libshape
{

class ChainShape;
class DrawPolylineOP;
class INodeCapture;
class IOperaterBase;

class EditPolylineImpl
{
public:
	EditPolylineImpl(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl, 
		d2d::PropertySettingPanel* propertyPanel, INodeCapture* node_capture, 
		DrawPolylineOP* draw_op, d2d::AbstractEditOP* select_op, IOperaterBase* base_op);

	bool OnKeyDown(int keyCode);
	bool OnMouseLeftDown(int x, int y);
	bool OnMouseLeftUp(int x, int y);
	bool OnMouseRightDown(int x, int y);
	bool OnMouseMove(int x, int y);
	bool OnMouseDrag(int x, int y);

	void OnDraw(const d2d::Screen& scr) const;

	void Clear();

private:
	void drawCaptured(const d2d::Screen& scr, const NodeAddr& captured) const;
	void checkActiveShape(const NodeAddr& captured);

private:
	class InterruptChainVisitor : public d2d::IVisitor
	{
	public:
		InterruptChainVisitor(const d2d::Vector& pos, int tol);

		virtual void visit(d2d::Object* object, bool& bFetchNext);

		ChainShape* getInterruptedChain() { return m_chain; }

	private:
		const d2d::Vector& m_pos;
		int m_tol;
		ChainShape* m_chain;

	}; // InterruptChainVisitor

	class NearestNodeVisitor : public d2d::IVisitor
	{
	public:
		NearestNodeVisitor(const d2d::Vector& pos, int tol);

		virtual void visit(d2d::Object* object, bool& bFetchNext);

		const d2d::Vector& getNearestNode() const {
			return m_nearest;
		}

	private:
		d2d::Vector m_pos;
		int m_tol;

		float m_dis;
		d2d::Vector m_nearest;

	}; // NearestNodeVisitor

private:
	static const int DRAG_SELECT_TOL = 5;

private:
	d2d::MultiShapesImpl* m_shapesImpl;

	d2d::PropertySettingPanel* m_propertyPanel;

	//		NodeAddr m_captured;

	DrawPolylineOP* m_draw_op;

	IOperaterBase* m_base_op;

	d2d::EditPanel* m_editPanel;

protected:
	NodeAddr m_capturedEditable, m_captureSelectable;

protected:
	d2d::AbstractEditOP* m_selectOP;

private:
	d2d::Vector m_lastLeftDownPos;
	bool m_bSelectOpen;

	INodeCapture* m_node_capture;

}; // EditPolylineImpl

}

#endif // _LIBSHAPE_EDIT_POLYLINE_IMPL_H_