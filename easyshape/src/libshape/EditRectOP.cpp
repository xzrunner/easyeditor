#include "EditRectOP.h"
#include "RectShape.h"
#include "RectPropertySetting.h"

#include <ee/MultiShapesImpl.h>
#include <ee/EditPanelImpl.h>
#include <ee/shape_msg.h>
#include <ee/ShapeSelection.h>
#include <ee/OneFloatValue.h>
#include <ee/panel_msg.h>
#include <ee/PrimitiveDraw.h>
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

	m_first_press = m_curr_pos = m_stage->TransPosScrToProj(x, y);

	m_shapes_impl->GetShapeSelection()->Clear();

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapes_impl, tolerance);
		capture.captureEditable(m_first_press, m_captured);

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
		if (m_first_press.IsValid())
		{
			m_curr_pos = m_stage->TransPosScrToProj(x, y);

			const float dis = ee::Math2D::GetDistance(m_first_press, m_curr_pos);
			if (dis > 1)
			{
				RectShape* rect = new RectShape(m_first_press, m_curr_pos);
				ee::SelectShapeSJ::Instance()->Select(rect);
				ee::InsertShapeSJ::Instance()->Insert(NULL);
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

bool EditRectOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_curr_pos = m_stage->TransPosScrToProj(x, y);

	if (m_captured.shape)
	{
		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
		{
			ee::Vector center(rect->m_rect.CenterX(), rect->m_rect.CenterY());

			// move
			if (!m_captured.pos.IsValid())
			{
				rect->m_rect.Translate(m_curr_pos - center);
			}
			// change size
			else 
			{
				if (m_captured.pos.x > center.x)
					rect->m_rect.xmax = m_curr_pos.x;
				else
					rect->m_rect.xmin = m_curr_pos.x;
				if (m_captured.pos.y > center.y)
					rect->m_rect.ymax = m_curr_pos.y;
				else
					rect->m_rect.ymin = m_curr_pos.y;

				m_captured.pos = m_curr_pos;
			}

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
				ee::Vector pos(rect->m_rect.CenterX(), rect->m_rect.CenterY());
				ee::PrimitiveDraw::DrawCircle(pos, tolerance, true, 2, ee::Colorf(0.4f, 1.0f, 0.4f));
				if (m_captured.pos.IsValid()) {
					ee::PrimitiveDraw::DrawCircle(m_captured.pos, tolerance, true, 2, ee::Colorf(1.0f, 0.4f, 0.4f));
				}
			}
		}
	}
	else
	{
		if (m_first_press.IsValid() && m_curr_pos.IsValid())
			ee::PrimitiveDraw::DrawRect(m_first_press, m_curr_pos, m_style);
	}

	return false;
}

bool EditRectOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_first_press.SetInvalid();
	m_curr_pos.SetInvalid();

	return false;
}

}