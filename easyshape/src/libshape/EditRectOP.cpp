#include "EditRectOP.h"
#include "RectShape.h"
#include "RectPropertySetting.h"

#include <ee/MultiShapesImpl.h>
#include <ee/EditPanelImpl.h>
#include <ee/shape_msg.h>
#include <ee/ShapeSelection.h>
#include <ee/OneFloatValue.h>
#include <ee/panel_msg.h>
#include <ee/EE_RVG.h>
#include <ee/Math2D.h>
#include <ee/PropertySettingPanel.h>

namespace eshape
{

EditRectOP::EditRectOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
					   ee::MultiShapesImpl* shapes_impl,
					   ee::PropertySettingPanel* property,
					   ee::OneFloatValue* node_capture)
	: ee::ZoomViewOP(wnd, stage, true)
	, m_property(property)
	, m_shapes_impl(shapes_impl)
	, m_node_capture(node_capture)
	, m_first_pos_valid(false)
	, m_curr_pos_valid(false)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);

	Clear();
}

bool EditRectOP::OnKeyDown(int keyCode)
{
	if (ee::ZoomViewOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapes_impl->ClearSelectedShape();
		m_captured.clear();
		ee::SelectShapeSJ::Instance()->Select(NULL);
	}

	return false;
}

bool EditRectOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_first_pos = m_curr_pos = m_stage->TransPosScrToProj(x, y);
	m_first_pos_valid = m_curr_pos_valid = true;

	m_shapes_impl->GetShapeSelection()->Clear();

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapes_impl, tolerance);
		capture.captureEditable(m_first_pos, m_captured);

		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
		{
			m_shapes_impl->GetShapeSelection()->Add(rect);
			ee::SelectShapeSJ::Instance()->Select(rect);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditRectOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		if (m_first_pos_valid)
		{
			m_curr_pos = m_stage->TransPosScrToProj(x, y);
			m_curr_pos_valid = true;

			const float dis = ee::Math2D::GetDistance(m_first_pos, m_curr_pos);
			if (dis > 1)
			{
				RectShape* rect = new RectShape(sm::rect(m_first_pos, m_curr_pos));
				ee::SelectShapeSJ::Instance()->Select(rect);
				ee::InsertShapeSJ::Instance()->Insert(rect);
				rect->Release();
			}
		}
	}
	else
	{
		m_property->EnablePropertyGrid(true);
		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape)) {
			ee::SelectShapeSJ::Instance()->Select(rect);
		}
	}

	Clear();


	return false;
}

bool EditRectOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y)) return true;

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

bool EditRectOP::OnMouseMove(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseMove(x, y)) return true;

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

bool EditRectOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_curr_pos = m_stage->TransPosScrToProj(x, y);
	m_curr_pos_valid = true;

	if (m_captured.shape)
	{
		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
		{
			sm::vec2 center = rect->GetRect().Center();

			sm::rect r = rect->GetRect();
			// move
			if (!m_captured.pos_valid)
			{
				r.Translate(m_curr_pos - center);
			}
			// change size
			else 
			{
				if (m_captured.pos.x > center.x)
					r.xmax = m_curr_pos.x;
				else
					r.xmin = m_curr_pos.x;
				if (m_captured.pos.y > center.y)
					r.ymax = m_curr_pos.y;
				else
					r.ymin = m_curr_pos.y;

				m_captured.pos = m_curr_pos;
			}
			rect->SetRect(r);

			m_property->EnablePropertyGrid(false);

			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}


	return false;
}

bool EditRectOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	if (m_captured.shape)
	{
		if (m_node_capture)
		{
			int tolerance = m_node_capture->GetValue();
			if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
			{
				sm::vec2 pos = rect->GetRect().Center();
				ee::RVG::Color(s2::Color(102, 255, 102));
				ee::RVG::Circle(pos, tolerance, true);
				if (m_captured.pos_valid) {
					ee::RVG::Color(s2::Color(255, 102, 102));
					ee::RVG::Circle(m_captured.pos, tolerance, true);
				}
			}
		}
	}
	else
	{
		if (m_first_pos_valid && m_curr_pos_valid)
			ee::RVG::Rect(m_first_pos, m_curr_pos, false);
	}

	return false;
}

bool EditRectOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_first_pos_valid = m_curr_pos_valid = false;

	return false;
}

}