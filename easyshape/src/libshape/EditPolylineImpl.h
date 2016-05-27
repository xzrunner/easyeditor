#ifndef _EASYSHAPE_EDIT_POLYLINE_IMPL_H_
#define _EASYSHAPE_EDIT_POLYLINE_IMPL_H_

#include "NodeCapture.h"

namespace ee { class EditOP; class EditPanelImpl; class PropertySettingPanel; class OneFloatValue; }

class wxWindow;

namespace eshape
{

class ChainShape;
class DrawPolylineOP;
class IOperaterBase;
class PolylineShape;

class EditPolylineImpl
{
public:
	EditPolylineImpl(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl, 
		ee::PropertySettingPanel* property, ee::OneFloatValue* node_capture, DrawPolylineOP* draw_op, 
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
		InterruptChainVisitor(const sm::vec2& pos, int tol);

		virtual void Visit(ee::Object* object, bool& next);

		PolylineShape* GetInterruptedPolyline() { return m_polyline; }

	private:
		const sm::vec2& m_pos;
		int m_tol;
		PolylineShape* m_polyline;

	}; // InterruptChainVisitor

	class NearestNodeVisitor : public ee::Visitor
	{
	public:
		NearestNodeVisitor(const sm::vec2& pos, int tol);

		virtual void Visit(ee::Object* object, bool& next);

		const sm::vec2& GetNearestNode() const {
			return m_nearest;
		}

	private:
		sm::vec2 m_pos;
		int m_tol;

		float m_dis;
		sm::vec2 m_nearest;
		bool m_nearest_valid;

	}; // NearestNodeVisitor

private:
	static const int DRAG_SELECT_TOL = 5;

private:
	ee::MultiShapesImpl* m_shapes_impl;

	ee::PropertySettingPanel* m_property;

	//		NodeAddr m_captured;

	DrawPolylineOP* m_draw_op;

	IOperaterBase* m_base_op;

	ee::EditPanelImpl* m_stage;

protected:
	NodeAddr m_captured_editable, m_captureSelectable;

protected:
	ee::EditOP* m_selectOP;

private:
	sm::vec2 m_last_left_down_pos;
	bool m_last_left_down_pos_valid;

	bool m_is_select_open;

	ee::OneFloatValue* m_node_capture;

	bool m_dirty;

}; // EditPolylineImpl

}

#endif // _EASYSHAPE_EDIT_POLYLINE_IMPL_H_