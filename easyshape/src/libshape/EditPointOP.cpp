#include "EditPointOP.h"
#include "PointShape.h"
#include "ShapeType.h"

namespace eshape
{

EditPointOP::EditPointOP(wxWindow* wnd, ee::EditPanelImpl* stage,
						 ee::MultiShapesImpl* shapes_impl,
						 ee::OneFloatValue* node_capture)
	: ee::ZoomViewOP(wnd, stage, true)
	, m_shapes_impl(shapes_impl)
	, m_node_capture(node_capture)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);

	Clear();
}

bool EditPointOP::OnKeyDown(int keyCode)
{
	if (ee::ZoomViewOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapes_impl->ClearSelectedShape();
		m_captured.clear();

	}

	return false;
}

bool EditPointOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_pos = m_stage->TransPosScrToProj(x, y);

	m_shapes_impl->GetShapeSelection()->Clear();
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapes_impl, tolerance);
		capture.captureEditable(m_pos, m_captured);
		if (m_captured.shape &&
			get_shape_type(m_captured.shape->GetShapeDesc()) == ST_POINT) {
			m_shapes_impl->GetShapeSelection()->Add(m_captured.shape);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditPointOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_pos.IsValid()) {
		return false;
	}

	m_pos = m_stage->TransPosScrToProj(x, y);
	if (!m_captured.shape) {
		PointShape* point = new PointShape(m_pos);
		m_shapes_impl->GetShapeSelection()->Add(point);
		ee::InsertShapeSJ::Instance()->Insert(point);
	}

	Clear();


	return false;
}

bool EditPointOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y)) return true;

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance == 0) {
		return false;
	}

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	NodeCapture capture(m_shapes_impl, tolerance);
	capture.captureEditable(pos, m_captured);
	if (m_captured.shape) {
		ee::RemoveShapeSJ::Instance()->Remove(m_captured.shape);
		m_shapes_impl->GetShapeSelection()->Clear();
		m_captured.clear();

	}

	return false;
}

bool EditPointOP::OnMouseMove(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseMove(x, y)) return true;

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance == 0) {
		return false;
	}

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	NodeCapture capture(m_shapes_impl, tolerance);
	ee::Shape* old = m_captured.shape;
	capture.captureEditable(pos, m_captured);
	if (old && !m_captured.shape || !old && m_captured.shape) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditPointOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_pos = m_stage->TransPosScrToProj(x, y);
	if (m_captured.shape && 
		get_shape_type(m_captured.shape->GetShapeDesc()) == ST_POINT) {		
		PointShape* point = static_cast<PointShape*>(m_captured.shape);
		point->SetPos(m_pos);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditPointOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	if (m_pos.IsValid()) {
		ee::PrimitiveDraw::DrawCircle(m_pos, m_node_capture->GetValue(), true, 2, ee::LIGHT_RED);
	}

	return false;
}

bool EditPointOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_pos.SetInvalid();

	return false;
}

}