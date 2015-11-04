#include "EditCircleOP.h"
#include "CircleShape.h"
#include "CirclePropertySetting.h"

namespace libshape
{

EditCircleOP::EditCircleOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
						   d2d::MultiShapesImpl* shapesImpl, 
						   d2d::PropertySettingPanel* propertyPanel, 
						   d2d::OneFloatValue* node_capture)
	: ZoomViewOP(wnd, stage, true)
	, m_propertyPanel(propertyPanel)
	, m_shapesImpl(shapesImpl)
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
		m_shapesImpl->ClearShapeSelection();
		m_captured.clear();


		if (m_view_panel_mgr) {
			m_view_panel_mgr->SelectShape(NULL, m_shapesImpl);
		}
	}

	return false;
}

bool EditCircleOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_firstPress = m_currPos = m_stage->TransPosScrToProj(x, y);

	m_shapesImpl->GetShapeSelection()->Clear();

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{
		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_firstPress, m_captured);

		if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
		{
			m_shapesImpl->GetShapeSelection()->Add(circle);
			if (m_view_panel_mgr) {
				m_view_panel_mgr->SelectShape(circle, m_shapesImpl);
			}
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
		if (m_firstPress.isValid())
		{
			m_currPos = m_stage->TransPosScrToProj(x, y);

			const float radius = d2d::Math::getDistance(m_firstPress, m_currPos);
			if (radius > 0)
			{
				CircleShape* circle = new CircleShape(m_firstPress, radius);
				if (m_view_panel_mgr) {
					m_view_panel_mgr->SelectShape(circle, m_shapesImpl);
				}
				m_shapesImpl->GetShapeSelection()->Add(circle);
				m_shapesImpl->InsertShape(circle);
			}
		}
	}
	else
	{
		if (m_propertyPanel) {
			m_propertyPanel->EnablePropertyGrid(true);
			if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape)) {
				if (m_view_panel_mgr) {
					m_view_panel_mgr->SelectShape(circle, m_shapesImpl);
				}
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
		m_currPos = m_stage->TransPosScrToProj(x, y);

		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_currPos, m_captured);
		if (m_captured.shape)
		{
			m_shapesImpl->RemoveShape(m_captured.shape);
			m_shapesImpl->GetShapeSelection()->Clear();
			m_captured.clear();


			if (m_view_panel_mgr) {
				m_view_panel_mgr->SelectShape(NULL, m_shapesImpl);
			}
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

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		d2d::IShape* old = m_captured.shape;
		capture.captureEditable(pos, m_captured);
		if (old && !m_captured.shape || !old && m_captured.shape) {
			m_stage->SetCanvasDirty();
		}
	}

	return false;
}

bool EditCircleOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_currPos = m_stage->TransPosScrToProj(x, y);

	if (m_captured.shape)
	{
		if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
		{
			// move  
			if (m_captured.pos.isValid())
				circle->center = m_currPos;
			// change size
			else
				circle->radius = d2d::Math::getDistance(m_currPos, circle->center);

			if (m_propertyPanel) {
				m_propertyPanel->EnablePropertyGrid(false);
			}
		}
	}

	m_stage->SetCanvasDirty();

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
				d2d::PrimitiveDraw::drawCircle(circle->center, tolerance, 
					true, 2, d2d::Colorf(0.4f, 1.0f, 0.4f));
				if (!m_captured.pos.isValid()) {
					d2d::PrimitiveDraw::drawCircle(circle->center, circle->radius,
						false, tolerance, d2d::Colorf(1.0f, 0.4f, 0.4f));
				}
			}
		}
	}
	else
	{
		if (m_firstPress.isValid() && m_currPos.isValid()) {
			d2d::PrimitiveDraw::drawCircle(m_firstPress, d2d::Math::getDistance(m_firstPress, m_currPos), 
				false, 3, d2d::Colorf(0, 0, 0), 32);
		}
	}

	return false;
}

bool EditCircleOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_firstPress.setInvalid();
	m_currPos.setInvalid();

	return false;
}

}