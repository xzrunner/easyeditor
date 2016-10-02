#include "EditPolylineImpl.h"
#include "DrawPolylineOP.h"
#include "ChainShape.h"
#include "PolygonShape.h"

#include "PolygonPropertySetting.h"
#include "ChainPropertySetting.h"

#include "IOperaterBase.h"

#include <ee/OneFloatValue.h>
#include <ee/EditPanelImpl.h>
#include <ee/MultiShapesImpl.h>
#include <ee/Math2D.h>
#include <ee/PropertySettingPanel.h>
#include <ee/panel_msg.h>
#include <ee/ShapeSelection.h>
#include <ee/shape_msg.h>

#include <sprite2/S2_RVG.h>
#include <SM_Calc.h>

namespace eshape
{

EditPolylineImpl::EditPolylineImpl(wxWindow* wnd, ee::EditPanelImpl* stage,
								   ee::MultiShapesImpl* shapes_impl,
								   ee::PropertySettingPanel* property, 
								   ee::OneFloatValue* node_capture, 
								   DrawPolylineOP* draw_op,
								   ee::EditOP* select_op,
								   IOperaterBase* base_op)
{
	m_stage = stage;

	m_shapes_impl = shapes_impl;

	m_property = property;

	m_node_capture = node_capture;

	m_selectOP = select_op;
	m_last_left_down_pos.MakeInvalid();
	m_is_select_open = false;

	m_draw_op = draw_op;

	m_base_op = base_op;

	m_dirty = false;
}

bool EditPolylineImpl::OnKeyDown(int keyCode)
{
	if (keyCode == WXK_DELETE)
	{
		m_captured_editable.Clear();
		m_captureSelectable.Clear();
	}
	return m_selectOP->OnKeyDown(keyCode);
}

bool EditPolylineImpl::OnMouseLeftDown(int x, int y)
{
	if (m_draw_op->ShouldFixPos())
	{
		return m_base_op->OnMouseLeftDownBase(x, y);
	}
	else
	{
		int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
		if (tolerance != 0)
		{	
			NodeCapture capture(m_shapes_impl, tolerance);
			sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
			capture.captureEditable(pos, m_captured_editable);
			if (!m_captured_editable.shape)
				capture.captureSelectable(pos, m_captureSelectable);
		}

		if (m_captured_editable.shape)
		{
			// 			sm::vec2 screen = m_stage->transPosProjectToScreen(m_capturedEditable.pos);
			// 			bNotDeliver = m_draw_op->OnMouseLeftDown(screen.x, screen.y);

			if (m_captured_editable.pos.IsValid())
				m_draw_op->m_polyline.push_back(m_captured_editable.pos);

			checkActiveShape(m_captured_editable);
		}
		else if (m_captureSelectable.shape)
		{
			if (m_captureSelectable.pos.IsValid())
				m_draw_op->m_polyline.push_back(m_captureSelectable.pos);

			checkActiveShape(m_captureSelectable);
		}
		else
		{
			if (m_draw_op->m_polyline.empty()) {
				m_last_left_down_pos = sm::vec2(x, y);
			}

			if (tolerance != 0 && m_draw_op->m_polyline.empty())
			{
				sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
				InterruptChainVisitor interrupt(pos, tolerance);
				m_shapes_impl->TraverseShapes(interrupt, ee::DT_EDITABLE);
				if (interrupt.GetInterruptedPolyline())
				{
					m_captured_editable.shape = interrupt.GetInterruptedPolyline();
					m_captured_editable.pos = pos;
					ee::SetCanvasDirtySJ::Instance()->SetDirty();

					checkActiveShape(m_captured_editable);
				}
				else
				{
					int sz = m_shapes_impl->GetShapeSelection()->Size();

					sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
					ee::Shape* shape = m_shapes_impl->QueryShapeByPos(pos);

					return m_base_op->OnMouseLeftDownBase(x, y);
				}
			}
			else
			{
				return m_base_op->OnMouseLeftDownBase(x, y);
			}
		}
	}

	return false;
}

bool EditPolylineImpl::OnMouseLeftUp(int x, int y)
{
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		NearestNodeVisitor nearest(pos, tolerance);
		m_shapes_impl->TraverseShapes(nearest, ee::DT_VISIBLE);
		float dis = sm::dis_pos_to_pos(nearest.GetNearestNode(), pos);
		if (dis < tolerance)
		{
			if (m_captured_editable.shape)
			{
				EditedPolyShape* polyline = dynamic_cast<EditedPolyShape*>(m_captured_editable.shape);
				polyline->ChangeVertex(m_captured_editable.pos, nearest.GetNearestNode());
				m_dirty = true;
				m_captured_editable.pos = nearest.GetNearestNode();
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}

		if (m_captured_editable.shape)
		{
			if (m_property) {
				m_property->EnablePropertyGrid(true);
			}
			checkActiveShape(m_captured_editable);
		}
	}

	if (m_is_select_open)
	{
		m_selectOP->OnMouseLeftUp(x, y);
		m_is_select_open = false;
		m_last_left_down_pos.MakeInvalid();
	}


	return false;
}

bool EditPolylineImpl::OnMouseRightDown(int x, int y)
{
	if (m_draw_op->m_polyline.empty())
	{
		int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
		if (tolerance != 0)
		{
			NodeCapture capture(m_shapes_impl, tolerance);
			capture.captureEditable(m_stage->TransPosScrToProj(x, y), m_captured_editable);
			if (m_captured_editable.shape)
			{
				if (m_captured_editable.pos.IsValid())
				{
					EditedPolyShape* polyline = dynamic_cast<EditedPolyShape*>(m_captured_editable.shape);
					polyline->RemoveVertex(m_captured_editable.pos);
					m_dirty = true;
				}
				else
				{
					ee::RemoveShapeSJ::Instance()->Remove(m_captured_editable.shape);
					m_shapes_impl->GetShapeSelection()->Clear();
					m_captured_editable.Clear();
					m_captureSelectable.Clear();
					m_dirty = true;
					ee::SelectShapeSJ::Instance()->Select(NULL);
				}
				m_captured_editable.shape = NULL;
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}

		return false;
	}
	else
	{
		return m_base_op->OnMouseRightDownBase(x, y);
	}
}

bool EditPolylineImpl::OnMouseMove(int x, int y)
{
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		NodeCapture capture(m_shapes_impl, tolerance);
		{
			ee::Shape* old = m_captured_editable.shape;
			capture.captureEditable(pos, m_captured_editable);
			if (old && !m_captured_editable.shape || !old && m_captured_editable.shape) {
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		{
			ee::Shape* old = m_captureSelectable.shape;
			capture.captureSelectable(pos, m_captureSelectable);
			if (old && !m_captureSelectable.shape || !old && m_captureSelectable.shape) {
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
	}

	return false;
}

bool EditPolylineImpl::OnMouseDrag(int x, int y)
{
	if (m_is_select_open)
		m_selectOP->OnMouseDrag(x, y);
	else if (m_captured_editable.shape)
	{
		if (m_draw_op->m_polyline.size() > 1)
		{
			m_captured_editable.shape = NULL;
			return false;
		}

		if (m_draw_op->m_polyline.size() == 1)
			m_draw_op->m_polyline.clear();

		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		if (EditedPolyShape* polyline = dynamic_cast<EditedPolyShape*>(m_captured_editable.shape))
		{
			if (m_captured_editable.pos.IsValid())
			{
				polyline->ChangeVertex(m_captured_editable.pos, pos);
				m_captured_editable.pos = pos;
			}
			else
			{
				sm::vec2 old = polyline->GetBounding().Center();
				polyline->Translate(pos - old);
			}

			m_dirty = true;
			ee::SetCanvasDirtySJ::Instance()->SetDirty();

			if (m_property) {
				m_property->EnablePropertyGrid(false);
			}
		}
	}
	else if (m_last_left_down_pos.IsValid()
		&& sm::dis_pos_to_pos(m_last_left_down_pos, sm::vec2(x, y)) < DRAG_SELECT_TOL)
		//		&& (m_lastLeftDownPos.x != x || m_lastLeftDownPos.y != y))
	{
		if (m_draw_op->m_polyline.size() == 1)
			m_draw_op->m_polyline.clear();
		m_is_select_open = true;
		m_selectOP->OnMouseLeftDown(x, y);
	}

	return false;
}

void EditPolylineImpl::OnDraw() const
{
	m_selectOP->OnDraw();
	if (m_node_capture)
	{
		if (m_captured_editable.shape) {
			drawCaptured(m_captured_editable);
		} else if (m_captureSelectable.shape) {
			drawCaptured(m_captureSelectable);
		}
	}
}

void EditPolylineImpl::Clear()
{
	m_selectOP->Clear();
	m_captured_editable.shape = NULL;
	m_captureSelectable.shape = NULL;
}

void EditPolylineImpl::drawCaptured(const NodeAddr& captured) const
{
	if (EditedPolyShape* polyline = dynamic_cast<EditedPolyShape*>(captured.shape))
	{
		if (captured.pos.IsValid()) {
			s2::RVG::SetColor(s2::Color(255, 102, 102));
			s2::RVG::Circle(captured.pos, m_node_capture->GetValue(), true);
		}

		sm::vec2 center = polyline->GetBounding().Center();
		s2::RVG::SetColor(s2::Color(102, 255, 102));
		s2::RVG::Circle(center, m_node_capture->GetValue(), true);
	}
}

void EditPolylineImpl::checkActiveShape(const NodeAddr& captured)
{
	ee::SelectShapeSJ::Instance()->Select(captured.shape);

	ee::ShapeSelection* selection = m_shapes_impl->GetShapeSelection();
	selection->Clear();
	selection->Add(captured.shape);
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylineImpl::InterruptChainVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylineImpl::InterruptChainVisitor::
InterruptChainVisitor(const sm::vec2& pos, int tol)
	: m_pos(pos)
	, m_tol(tol)
	, m_polyline(NULL)
{
}

void EditPolylineImpl::InterruptChainVisitor::
Visit(ee::Shape* shape, bool& next) 
{
	EditedPolyShape* polyline = dynamic_cast<EditedPolyShape*>(shape);
	if (!polyline) {
		next = true;
		return;
	}

	sm::rect rect(m_pos, m_tol, m_tol);
	if (!polyline->IsIntersect(rect)) 
	{
		next = true;
		return;
	}

	size_t idx;
	const std::vector<sm::vec2>& vertices = polyline->GetVertices();
	float dis = ee::Math2D::GetDisPointToPolyline(m_pos, vertices, &idx);
	if (dis < m_tol)
	{
		polyline->AddVertex(idx + 1, m_pos);
		m_polyline = polyline;
		next = false;
		return;
	}
	else if (polyline->IsClosed() && vertices.size() > 1)
	{
		float dis = sm::dis_pos_to_seg(m_pos, vertices.front(), vertices.back());
		if (dis < m_tol)
		{
			polyline->AddVertex(vertices.size(), m_pos);
			m_polyline = polyline;
			next = false;
			return;
		}
	}

	next = true;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylineImpl::NearestNodeVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylineImpl::NearestNodeVisitor::
NearestNodeVisitor(const sm::vec2& pos, int tol)
	: m_pos(pos)
	, m_tol(tol)
{
	m_nearest.MakeInvalid();
	m_dis = FLT_MAX;
}

void EditPolylineImpl::NearestNodeVisitor::
Visit(ee::Shape* shape, bool& next)
{
	EditedPolyShape* polyline = dynamic_cast<EditedPolyShape*>(shape);
	if (!polyline) 
	{
		next = true;
		return;
	}

	sm::rect rect(m_pos, m_tol, m_tol);

	if (!polyline->IsIntersect(rect)) 
	{
		next = true;
		return;
	}

	const std::vector<sm::vec2>& vertices = polyline->GetVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		float dis = sm::dis_pos_to_pos(m_pos, vertices[i]);
		if (dis < m_dis && dis != 0)
		{
			m_dis = dis;
			m_nearest = vertices[i];
		}
	}

	next = true;
}

}