#ifndef _LIBSHAPE_EDIT_POLYLINE_IMPL_H_
#define _LIBSHAPE_EDIT_POLYLINE_IMPL_H_

#include <drag2d.h>

#include "NodeCapture.h"

namespace libshape
{

class ChainShape;
class DrawPolylineOP;
class d2d::OneFloatValue;
class IOperaterBase;

class EditPolylineImpl
{
public:
	EditPolylineImpl(wxWindow* wnd, d2d::EditPanelImpl* stage, d2d::MultiShapesImpl* shapesImpl, 
		d2d::PropertySettingPanel* propertyPanel, d2d::OneFloatValue* node_capture, DrawPolylineOP* draw_op, 
		d2d::AbstractEditOP* select_op, IOperaterBase* base_op);

	bool OnKeyDown(int keyCode);
	bool OnMouseLeftDown(int x, int y);
	bool OnMouseLeftUp(int x, int y);
	bool OnMouseRightDown(int x, int y);
	bool OnMouseMove(int x, int y);
	bool OnMouseDrag(int x, int y);

	void OnDraw() const;

	void Clear();

	bool IsDirty() const { return m_dirty; }
	void ResetDirty() { m_dirty = false; }

private:
	void drawCaptured(const NodeAddr& captured) const;
	void checkActiveShape(const NodeAddr& captured);

private:
	class InterruptChainVisitor : public d2d::Visitor
	{
	public:
		InterruptChainVisitor(const d2d::Vector& pos, int tol);

		virtual void Visit(d2d::Object* object, bool& next);

		ChainShape* getInterruptedChain() { return m_chain; }

	private:
		const d2d::Vector& m_pos;
		int m_tol;
		ChainShape* m_chain;

	}; // InterruptChainVisitor

	class NearestNodeVisitor : public d2d::Visitor
	{
	public:
		NearestNodeVisitor(const d2d::Vector& pos, int tol);

		virtual void Visit(d2d::Object* object, bool& next);

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

	d2d::EditPanelImpl* m_stage;

protected:
	NodeAddr m_capturedEditable, m_captureSelectable;

protected:
	d2d::AbstractEditOP* m_selectOP;

private:
	d2d::Vector m_lastLeftDownPos;
	bool m_bSelectOpen;

	d2d::OneFloatValue* m_node_capture;

	bool m_dirty;

}; // EditPolylineImpl

}

#endif // _LIBSHAPE_EDIT_POLYLINE_IMPL_H_