#ifndef _LIBSHAPE_EDIT_POLYLINE_IMPL_H_
#define _LIBSHAPE_EDIT_POLYLINE_IMPL_H_



#include "NodeCapture.h"

namespace libshape
{

class ChainShape;
class DrawPolylineOP;
class ee::OneFloatValue;
class IOperaterBase;

class EditPolylineImpl
{
public:
	EditPolylineImpl(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl, 
		ee::PropertySettingPanel* propertyPanel, ee::OneFloatValue* node_capture, DrawPolylineOP* draw_op, 
		ee::EditOP* select_op, IOperaterBase* base_op);

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
	class InterruptChainVisitor : public ee::Visitor
	{
	public:
		InterruptChainVisitor(const ee::Vector& pos, int tol);

		virtual void Visit(ee::Object* object, bool& next);

		ChainShape* getInterruptedChain() { return m_chain; }

	private:
		const ee::Vector& m_pos;
		int m_tol;
		ChainShape* m_chain;

	}; // InterruptChainVisitor

	class NearestNodeVisitor : public ee::Visitor
	{
	public:
		NearestNodeVisitor(const ee::Vector& pos, int tol);

		virtual void Visit(ee::Object* object, bool& next);

		const ee::Vector& getNearestNode() const {
			return m_nearest;
		}

	private:
		ee::Vector m_pos;
		int m_tol;

		float m_dis;
		ee::Vector m_nearest;

	}; // NearestNodeVisitor

private:
	static const int DRAG_SELECT_TOL = 5;

private:
	ee::MultiShapesImpl* m_shapesImpl;

	ee::PropertySettingPanel* m_propertyPanel;

	//		NodeAddr m_captured;

	DrawPolylineOP* m_draw_op;

	IOperaterBase* m_base_op;

	ee::EditPanelImpl* m_stage;

protected:
	NodeAddr m_capturedEditable, m_captureSelectable;

protected:
	ee::EditOP* m_selectOP;

private:
	ee::Vector m_lastLeftDownPos;
	bool m_bSelectOpen;

	ee::OneFloatValue* m_node_capture;

	bool m_dirty;

}; // EditPolylineImpl

}

#endif // _LIBSHAPE_EDIT_POLYLINE_IMPL_H_