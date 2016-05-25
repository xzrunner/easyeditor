#include "EditBezierOP.h"
#include "BezierShape.h"
#include "BezierPropertySetting.h"

#include <ee/MultiShapesImpl.h>
#include <ee/EditPanelImpl.h>
#include <ee/shape_msg.h>
#include <ee/Math2D.h>
#include <ee/ShapeSelection.h>
#include <ee/OneFloatValue.h>
#include <ee/PropertySettingPanel.h>
#include <ee/EE_RVG.h>
#include <ee/panel_msg.h>

namespace eshape
{

EditBezierOP::EditBezierOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
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

bool EditBezierOP::OnKeyDown(int keyCode)
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

bool EditBezierOP::OnMouseLeftDown(int x, int y)
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
 		if (BezierShape* bezier = dynamic_cast<BezierShape*>(m_captured.shape))
		{
			m_shapes_impl->GetShapeSelection()->Add(bezier);
			ee::SelectShapeSJ::Instance()->Select(bezier);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditBezierOP::OnMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		if (m_first_pos_valid)
		{
			m_curr_pos = m_stage->TransPosScrToProj(x, y);
			m_curr_pos_valid = true;

			const float dis = ee::Math2D::GetDistance(m_first_pos, m_curr_pos);
			if (dis > 1)
			{
				BezierShape* bezier = new BezierShape(m_first_pos, m_curr_pos);
				ee::SelectShapeSJ::Instance()->Select(bezier);
				ee::InsertShapeSJ::Instance()->Insert(bezier);
				bezier->Release();
			}
		}
	}
	else
	{
 		m_property->EnablePropertyGrid(true);
		ee::SelectShapeSJ::Instance()->Select(m_captured.shape);
	}

	Clear();


	return false;
}

bool EditBezierOP::OnMouseRightDown(int x, int y)
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

bool EditBezierOP::OnMouseMove(int x, int y)
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

bool EditBezierOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_curr_pos = m_stage->TransPosScrToProj(x, y);
	m_curr_pos_valid = true;

	if (m_captured.shape)
	{
		if (BezierShape* bezier = dynamic_cast<BezierShape*>(m_captured.shape))
		{
			sm::vec2 center = bezier->GetRect().Center();

			if (!m_captured.pos_valid) {
				bezier->Translate(m_curr_pos - center);
			} else {
				bezier->MoveCtrlNode(m_captured.pos, m_curr_pos);
				m_captured.pos = m_curr_pos;
				m_captured.pos_valid = true;
			}

			m_property->EnablePropertyGrid(false);
		}
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool EditBezierOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	if (m_captured.shape)
	{
		if (m_node_capture)
		{
			int tolerance = m_node_capture->GetValue();
			if (BezierShape* bezier = dynamic_cast<BezierShape*>(m_captured.shape))
			{
				sm::vec2 c = bezier->GetRect().Center();
				
				ee::RVG::Color(s2::Color(102, 255, 102));
				ee::RVG::Circle(c, tolerance, true);
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
		{
			BezierShape bezier(m_first_pos, m_curr_pos);
			bezier.Draw(sm::mat4());
		}
	}

	return false;
}

bool EditBezierOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_first_pos_valid = false;
	m_curr_pos_valid = false;

	return false;
}

}