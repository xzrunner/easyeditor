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
#include <ee/Matrix.h>

namespace eshape
{

EditBezierOP::EditBezierOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
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

	m_firstPress = m_curr_pos = m_stage->TransPosScrToProj(x, y);

	m_shapes_impl->GetShapeSelection()->Clear();

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapes_impl, tolerance);
		capture.captureEditable(m_firstPress, m_captured);
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
		if (m_firstPress.IsValid())
		{
			m_curr_pos = m_stage->TransPosScrToProj(x, y);

			const float dis = ee::Math2D::GetDistance(m_firstPress, m_curr_pos);
			if (dis > 1)
			{
				BezierShape* bezier = new BezierShape(m_firstPress, m_curr_pos);
				ee::SelectShapeSJ::Instance()->Select(bezier);
				ee::InsertShapeSJ::Instance()->Insert(bezier);
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

bool EditBezierOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_curr_pos = m_stage->TransPosScrToProj(x, y);

	if (m_captured.shape)
	{
		if (BezierShape* bezier = dynamic_cast<BezierShape*>(m_captured.shape))
		{
			ee::Vector center(bezier->GetRect().CenterX(), bezier->GetRect().CenterY());

			if (!m_captured.pos.IsValid()) {
				bezier->Translate(m_curr_pos - center);
			} else {
				bezier->MoveCtrlNode(m_captured.pos, m_curr_pos);
				m_captured.pos = m_curr_pos;
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
				ee::Vector c(bezier->GetRect().CenterX(), bezier->GetRect().CenterY());
				
				ee::RVG::Color(ee::Colorf(0.4f, 1.0f, 0.4f));
				ee::RVG::Circle(c, tolerance, true);
				if (m_captured.pos.IsValid()) {
					ee::RVG::Color(ee::Colorf(1.0f, 0.4f, 0.4f));
					ee::RVG::Circle(m_captured.pos, tolerance, true);
				}
			}
		}
	}
	else
	{
		if (m_firstPress.IsValid() && m_curr_pos.IsValid())
		{
			BezierShape bezier(m_firstPress, m_curr_pos);
			bezier.Draw(ee::Matrix());
		}
	}

	return false;
}

bool EditBezierOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_firstPress.SetInvalid();
	m_curr_pos.SetInvalid();

	return false;
}

}