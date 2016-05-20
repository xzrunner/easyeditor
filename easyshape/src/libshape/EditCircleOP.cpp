#include "EditCircleOP.h"
#include "CircleShape.h"
#include "CirclePropertySetting.h"

#include <ee/MultiShapesImpl.h>
#include <ee/shape_msg.h>
#include <ee/EditPanelImpl.h>
#include <ee/OneFloatValue.h>
#include <ee/EditPanelImpl.h>
#include <ee/ShapeSelection.h>
#include <ee/Math2D.h>
#include <ee/EE_RVG.h>
#include <ee/PropertySettingPanel.h>
#include <ee/panel_msg.h>

namespace eshape
{

EditCircleOP::EditCircleOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
						   ee::MultiShapesImpl* shapes_impl, 
						   ee::PropertySettingPanel* property, 
						   ee::OneFloatValue* node_capture)
	: ZoomViewOP(wnd, stage, true)
	, m_property(property)
	, m_shapes_impl(shapes_impl)
	, m_node_capture(node_capture)
	, m_first_pos_valid(false)
	, m_curr_pos_valid(false)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);

	Clear();
}

bool EditCircleOP::OnKeyDown(int keyCode)
{
	if (ZoomViewOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapes_impl->ClearSelectedShape();
		m_captured.clear();
		ee::SelectShapeSJ::Instance()->Select(NULL);
	}

	return false;
}

bool EditCircleOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_first_pos = m_curr_pos = m_stage->TransPosScrToProj(x, y);
	m_first_pos_valid = m_curr_pos_valid = true;

	m_shapes_impl->GetShapeSelection()->Clear();

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{
		NodeCapture capture(m_shapes_impl, tolerance);
		capture.captureEditable(m_first_pos, m_captured);

		if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
		{
			m_shapes_impl->GetShapeSelection()->Add(circle);
			ee::SelectShapeSJ::Instance()->Select(circle);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditCircleOP::OnMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		if (m_first_pos_valid)
		{
			m_curr_pos = m_stage->TransPosScrToProj(x, y);
			m_curr_pos_valid = true;

			const float radius = ee::Math2D::GetDistance(m_first_pos, m_curr_pos);
			if (radius > 0)
			{
				CircleShape* circle = new CircleShape(m_first_pos, radius);
				ee::SelectShapeSJ::Instance()->Select(circle);
				ee::InsertShapeSJ::Instance()->Insert(NULL);
			}
		}
	}
	else
	{
		if (m_property) {
			m_property->EnablePropertyGrid(true);
			if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape)) {
				ee::SelectShapeSJ::Instance()->Select(circle);
			}
		}
	}

	Clear();


	return false;
}

bool EditCircleOP::OnMouseRightDown(int x, int y)
{
	if (ZoomViewOP::OnMouseRightDown(x, y)) return true;

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{
		m_curr_pos = m_stage->TransPosScrToProj(x, y);
		m_curr_pos_valid = true;

		NodeCapture capture(m_shapes_impl, tolerance);
		capture.captureEditable(m_curr_pos, m_captured);
		if (m_captured.shape)
		{
			ee::RemoveShapeSJ::Instance()->Remove(m_captured.shape);
			m_shapes_impl->GetShapeSelection()->Clear();
			m_captured.clear();
			ee::SelectShapeSJ::Instance()->Select(NULL);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditCircleOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapes_impl, tolerance);
		ee::Shape* old = m_captured.shape;
		capture.captureEditable(pos, m_captured);
		if (old && !m_captured.shape || !old && m_captured.shape) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}

	return false;
}

bool EditCircleOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_curr_pos = m_stage->TransPosScrToProj(x, y);
	m_curr_pos_valid = true;

	if (m_captured.shape)
	{
		if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
		{
			// move  
			if (m_captured.pos_valid)
				circle->SetCenter(m_curr_pos);
			// change size
			else
				circle->SetRadius(ee::Math2D::GetDistance(m_curr_pos, circle->GetCenter()));

			if (m_property) {
				m_property->EnablePropertyGrid(false);
			}
		}
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool EditCircleOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	if (m_captured.shape)
	{
		if (m_node_capture)
		{
			int tolerance = m_node_capture->GetValue();
			if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
			{
				ee::RVG::Color(s2::Color(102, 255, 102));
				ee::RVG::Circle(circle->GetCenter(), tolerance, true);
				if (!m_captured.pos_valid) {
					ee::RVG::Color(s2::Color(255, 102, 102));
					ee::RVG::Circle(circle->GetCenter(), circle->GetRadius(), false);
				}
			}
		}
	}
	else
	{
		if (m_first_pos_valid && m_curr_pos_valid) {
			ee::RVG::Color(s2::Color(0, 0, 0));
			ee::RVG::Circle(m_first_pos, ee::Math2D::GetDistance(m_first_pos, m_curr_pos), false, 32);
		}
	}

	return false;
}

bool EditCircleOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_first_pos_valid = m_curr_pos_valid = false;

	return false;
}

}