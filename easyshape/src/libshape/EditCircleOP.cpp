#include "EditCircleOP.h"
#include "CircleShape.h"
#include "CirclePropertySetting.h"

#include <ee/MultiShapesImpl.h>
#include <ee/shape_msg.h>
#include <ee/EditPanelImpl.h>
#include <ee/OneFloatValue.h>
#include <ee/EditPanelImpl.h>
#include <ee/ShapeSelection.h>
#include <ee/PropertySettingPanel.h>
#include <ee/panel_msg.h>

#include <sprite2/RVG.h>
#include <SM_Calc.h>

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
	m_first_pos.MakeInvalid();
	m_curr_pos.MakeInvalid();

	m_cursor = wxCursor(wxCURSOR_PENCIL);

	Clear();
}

bool EditCircleOP::OnKeyDown(int keyCode)
{
	if (ZoomViewOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapes_impl->ClearSelectedShape();
		m_captured.Clear();
		ee::SelectShapeSJ::Instance()->Select(NULL);
	}

	return false;
}

bool EditCircleOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_first_pos = m_curr_pos = m_stage->TransPosScrToProj(x, y);

	m_shapes_impl->GetShapeSelection()->Clear();

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{
		NodeCapture capture(m_shapes_impl, tolerance);
		capture.captureEditable(m_first_pos, m_captured);

		if (auto circle = std::dynamic_pointer_cast<CircleShape>(m_captured.shape))
		{
			m_shapes_impl->GetShapeSelection()->Add(circle);
			ee::SelectShapeSJ::Instance()->Select(circle);
		}
	}
	else
	{
		m_captured.Clear();
	}

	return false;
}

bool EditCircleOP::OnMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		if (m_first_pos.IsValid())
		{
			m_curr_pos = m_stage->TransPosScrToProj(x, y);

			const float radius = sm::dis_pos_to_pos(m_first_pos, m_curr_pos);
			if (radius > 0)
			{
				auto circle = std::make_shared<CircleShape>(m_first_pos, radius);
				ee::SelectShapeSJ::Instance()->Select(circle);
				ee::InsertShapeSJ::Instance()->Insert(circle);
			}
		}
	}
	else
	{
		if (m_property) {
			m_property->EnablePropertyGrid(true);
			if (auto circle = std::dynamic_pointer_cast<CircleShape>(m_captured.shape)) {
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
			m_captured.Clear();
			ee::SelectShapeSJ::Instance()->Select(NULL);
		}
	}
	else
	{
		m_captured.Clear();
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
		auto old = m_captured.shape;
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
		if (auto circle = std::dynamic_pointer_cast<CircleShape>(m_captured.shape))
		{
			// move  
			if (m_captured.pos.IsValid())
				circle->SetCenter(m_curr_pos);
			// change size
			else
				circle->SetRadius(sm::dis_pos_to_pos(m_curr_pos, circle->GetCenter()));

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
			if (auto circle = std::dynamic_pointer_cast<CircleShape>(m_captured.shape))
			{
				s2::RVG::SetColor(pt2::Color(102, 255, 102));
				s2::RVG::Circle(nullptr, circle->GetCenter(), tolerance, true);
				if (!m_captured.pos.IsValid()) {
					s2::RVG::SetColor(pt2::Color(255, 102, 102));
					s2::RVG::Circle(nullptr, circle->GetCenter(), circle->GetRadius(), false);
				}
			}
		}
	}
	else
	{
		if (m_first_pos.IsValid() && m_curr_pos.IsValid()) {
			s2::RVG::SetColor(pt2::Color(0, 0, 0));
			s2::RVG::Circle(nullptr, m_first_pos, sm::dis_pos_to_pos(m_first_pos, m_curr_pos), false, 32);
		}
	}

	return false;
}

bool EditCircleOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_first_pos.MakeInvalid();
	m_curr_pos.MakeInvalid();

	return false;
}

}