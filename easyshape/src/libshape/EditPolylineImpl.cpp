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
#include <ee/EE_RVG.h>
#include <ee/shape_msg.h>

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
	m_last_left_down_pos_valid = false;
	m_is_select_open = false;

	m_draw_op = draw_op;

	m_base_op = base_op;

	m_dirty = false;
}

bool EditPolylineImpl::OnKeyDown(int keyCode)
{
	if (keyCode == WXK_DELETE)
	{
		m_capturedEditable.clear();
		m_captureSelectable.clear();
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
			capture.captureEditable(pos, m_capturedEditable);
			if (!m_capturedEditable.shape)
				capture.captureSelectable(pos, m_captureSelectable);
		}

		if (m_capturedEditable.shape)
		{
			// 			sm::vec2 screen = m_stage->transPosProjectToScreen(m_capturedEditable.pos);
			// 			bNotDeliver = m_draw_op->OnMouseLeftDown(screen.x, screen.y);

			if (m_capturedEditable.pos_valid)
				m_draw_op->m_polyline.push_back(m_capturedEditable.pos);

			checkActiveShape(m_capturedEditable);
		}
		else if (m_captureSelectable.shape)
		{
			if (m_captureSelectable.pos_valid)
				m_draw_op->m_polyline.push_back(m_captureSelectable.pos);

			checkActiveShape(m_captureSelectable);
		}
		else
		{
			if (m_draw_op->m_polyline.empty()) {
				m_last_left_down_pos = sm::vec2(x, y);
				m_last_left_down_pos_valid = true;
			}

			if (tolerance != 0 && m_draw_op->m_polyline.empty())
			{
				sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
				InterruptChainVisitor interrupt(pos, tolerance);
				m_shapes_impl->TraverseShapes(interrupt, ee::DT_EDITABLE);
				if (interrupt.getInterruptedChain())
				{
					m_capturedEditable.shape = interrupt.getInterruptedChain();
					m_capturedEditable.pos = pos;
					ee::SetCanvasDirtySJ::Instance()->SetDirty();

					checkActiveShape(m_capturedEditable);
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
		float dis = ee::Math2D::GetDistance(nearest.getNearestNode(), pos);
		if (dis < tolerance)
		{
			if (m_capturedEditable.shape)
			{
				ChainShape* chain = dynamic_cast<ChainShape*>(m_capturedEditable.shape);
				chain->Change(m_capturedEditable.pos, nearest.getNearestNode());
				chain->refresh();
				m_dirty = true;
				m_capturedEditable.pos = nearest.getNearestNode();
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}

		if (m_capturedEditable.shape)
		{
			if (m_property) {
				m_property->EnablePropertyGrid(true);
			}
			checkActiveShape(m_capturedEditable);
		}
	}

	if (m_is_select_open)
	{
		m_selectOP->OnMouseLeftUp(x, y);
		m_is_select_open = false;
		m_last_left_down_pos_valid = false;
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
			capture.captureEditable(m_stage->TransPosScrToProj(x, y), m_capturedEditable);
			if (m_capturedEditable.shape)
			{
				if (m_capturedEditable.pos_valid)
				{
					ChainShape* chain = dynamic_cast<ChainShape*>(m_capturedEditable.shape);
					chain->Remove(m_capturedEditable.pos);
					chain->refresh();
					m_dirty = true;
				}
				else
				{
					ee::RemoveShapeSJ::Instance()->Remove(m_capturedEditable.shape);
					m_shapes_impl->GetShapeSelection()->Clear();
					m_capturedEditable.clear();
					m_captureSelectable.clear();
					m_dirty = true;
					ee::SelectShapeSJ::Instance()->Select(NULL);
				}
				m_capturedEditable.shape = NULL;
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
			ee::Shape* old = m_capturedEditable.shape;
			capture.captureEditable(pos, m_capturedEditable);
			if (old && !m_capturedEditable.shape || !old && m_capturedEditable.shape) {
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
	else if (m_capturedEditable.shape)
	{
		if (m_draw_op->m_polyline.size() > 1)
		{
			m_capturedEditable.shape = NULL;
			return false;
		}

		if (m_draw_op->m_polyline.size() == 1)
			m_draw_op->m_polyline.clear();

		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		if (ChainShape* chain = dynamic_cast<ChainShape*>(m_capturedEditable.shape))
		{
			if (m_capturedEditable.pos_valid)
			{
				chain->Change(m_capturedEditable.pos, pos);
				m_capturedEditable.pos = pos;
			}
			else
			{
				sm::vec2 old = chain->GetRect().Center();
				chain->Translate(pos - old);
			}

			chain->refresh();
			m_dirty = true;
			ee::SetCanvasDirtySJ::Instance()->SetDirty();

			if (m_property) {
				m_property->EnablePropertyGrid(false);
			}
		}
	}
	else if (m_last_left_down_pos_valid
		&& ee::Math2D::GetDistance(m_last_left_down_pos, sm::vec2(x, y)) < DRAG_SELECT_TOL)
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
		if (m_capturedEditable.shape) {
			drawCaptured(m_capturedEditable);
		} else if (m_captureSelectable.shape) {
			drawCaptured(m_captureSelectable);
		}
	}
}

void EditPolylineImpl::Clear()
{
	m_selectOP->Clear();
	m_capturedEditable.shape = NULL;
	m_captureSelectable.shape = NULL;
}

void EditPolylineImpl::drawCaptured(const NodeAddr& captured) const
{
	if (ChainShape* chain = dynamic_cast<ChainShape*>(captured.shape))
	{
		if (captured.pos_valid) {
			ee::RVG::Color(s2::Color(255, 102, 102));
			ee::RVG::Circle(captured.pos, m_node_capture->GetValue(), true);
		}

		sm::vec2 center = chain->GetRect().Center();
		ee::RVG::Color(s2::Color(102, 255, 102));
		ee::RVG::Circle(center, m_node_capture->GetValue(), true);
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
, m_chain(NULL)
{
}

void EditPolylineImpl::InterruptChainVisitor::
Visit(ee::Object* object, bool& next) 
{
	sm::rect rect(m_pos, m_tol, m_tol);

	ChainShape* chain = static_cast<ChainShape*>(object);
	if (!chain->IsIntersect(rect)) 
	{
		next = true;
		return;
	}

	size_t iPos;
	const std::vector<sm::vec2>& vertices = chain->GetVertices();
	float dis = ee::Math2D::GetDisPointToPolyline(m_pos, vertices, &iPos);
	if (dis < m_tol)
	{
		chain->Add(iPos + 1, m_pos);
		m_chain = chain;
		next = false;
		return;
	}
	else if (chain->IsClosed() && vertices.size() > 1)
	{
		float dis = ee::Math2D::GetDisPointToSegment(m_pos, vertices.front(), vertices.back());
		if (dis < m_tol)
		{
			chain->Add(vertices.size(), m_pos);
			m_chain = chain;
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
	, m_nearest_valid(false)
{
	m_dis = FLT_MAX;
}

void EditPolylineImpl::NearestNodeVisitor::
Visit(ee::Object* object, bool& next)
{
	ChainShape* chain = dynamic_cast<ChainShape*>(object);
	if (!chain) 
	{
		next = true;
		return;
	}

	sm::rect rect(m_pos, m_tol, m_tol);

	if (!chain->IsIntersect(rect)) 
	{
		next = true;
		return;
	}

	const std::vector<sm::vec2>& vertices = chain->GetVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		float dis = ee::Math2D::GetDistance(m_pos, vertices[i]);
		if (dis < m_dis && dis != 0)
		{
			m_dis = dis;
			m_nearest = vertices[i];
			m_nearest_valid = true;
		}
	}

	next = true;
}

}