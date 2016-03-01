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

	m_firstPress = m_curr_pos = m_stage->TransPosScrToProj(x, y);

	m_shapes_impl->GetShapeSelection()->Clear();

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{
		NodeCapture capture(m_shapes_impl, tolerance);
		capture.captureEditable(m_firstPress, m_captured);

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
		if (m_firstPress.IsValid())
		{
			m_curr_pos = m_stage->TransPosScrToProj(x, y);

			const float radius = ee::Math2D::GetDistance(m_firstPress, m_curr_pos);
			if (radius > 0)
			{
				CircleShape* circle = new CircleShape(m_firstPress, radius);
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

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
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

	if (m_captured.shape)
	{
		if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
		{
			// move  
			if (m_captured.pos.IsValid())
				circle->center = m_curr_pos;
			// change size
			else
				circle->radius = ee::Math2D::GetDistance(m_curr_pos, circle->center);

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
				ee::RVG::Circle(circle->center, tolerance, true, ee::Colorf(0.4f, 1.0f, 0.4f));
				if (!m_captured.pos.IsValid()) {
					ee::RVG::Circle(circle->center, circle->radius, false, ee::Colorf(1.0f, 0.4f, 0.4f));
				}
			}
		}
	}
	else
	{
		if (m_firstPress.IsValid() && m_curr_pos.IsValid()) {
			ee::RVG::Circle(m_firstPress, ee::Math2D::GetDistance(m_firstPress, m_curr_pos), false, ee::Colorf(0, 0, 0), 32);
		}
	}

	return false;
}

bool EditCircleOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_firstPress.SetInvalid();
	m_curr_pos.SetInvalid();

	return false;
}

}