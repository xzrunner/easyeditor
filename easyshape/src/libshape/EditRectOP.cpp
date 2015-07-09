#include "EditRectOP.h"
#include "RectShape.h"
#include "RectPropertySetting.h"

namespace libshape
{

EditRectOP::EditRectOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
					   d2d::PropertySettingPanel* propertyPanel,
					   d2d::ViewPanelMgr* view_panel_mgr,
					   d2d::OneFloatValue* node_capture)
	: d2d::ZoomViewOP(editPanel, true)
	, m_propertyPanel(propertyPanel)
	, m_view_panel_mgr(view_panel_mgr)
	, m_shapesImpl(shapesImpl)
	, m_node_capture(node_capture)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);

	Clear();
}

bool EditRectOP::OnKeyDown(int keyCode)
{
	if (d2d::ZoomViewOP::OnKeyDown(keyCode)) return true;

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

bool EditRectOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_firstPress = m_currPos = m_stage->TransPosScrToProj(x, y);

	m_shapesImpl->GetShapeSelection()->Clear();

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_firstPress, m_captured);

		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
		{
			m_shapesImpl->GetShapeSelection()->Add(rect);
			if (m_view_panel_mgr) {
				m_view_panel_mgr->SelectShape(rect, m_shapesImpl);
			}
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
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		if (m_firstPress.isValid())
		{
			m_currPos = m_stage->TransPosScrToProj(x, y);

			const float dis = d2d::Math::getDistance(m_firstPress, m_currPos);
			if (dis > 1)
			{
				RectShape* rect = new RectShape(m_firstPress, m_currPos);
				if (m_view_panel_mgr) {
					m_view_panel_mgr->SelectShape(rect, m_shapesImpl);
				}
				m_shapesImpl->GetShapeSelection()->Add(rect);
				m_shapesImpl->InsertShape(rect);
			}
		}
	}
	else
	{
		m_propertyPanel->EnablePropertyGrid(true);
		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape)) {
			if (m_view_panel_mgr) {
				m_view_panel_mgr->SelectShape(rect, m_shapesImpl);
			}
		}
	}

	Clear();

	return false;
}

bool EditRectOP::OnMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseRightDown(x, y)) return true;

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

bool EditRectOP::OnMouseMove(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseMove(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		d2d::IShape* old = m_captured.shape;
		capture.captureEditable(pos, m_captured);
		if (old && !m_captured.shape || !old && m_captured.shape) {
			m_stage->GetCanvas()->SetDirty();
		}
	}

	return false;
}

bool EditRectOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_currPos = m_stage->TransPosScrToProj(x, y);

	if (m_captured.shape)
	{
		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
		{
			d2d::Vector center(rect->m_rect.xCenter(), rect->m_rect.yCenter());

			// move
			if (!m_captured.pos.isValid())
			{
				rect->m_rect.translate(m_currPos - center);
			}
			// change size
			else 
			{
				if (m_captured.pos.x > center.x)
					rect->m_rect.xMax = m_currPos.x;
				else
					rect->m_rect.xMin = m_currPos.x;
				if (m_captured.pos.y > center.y)
					rect->m_rect.yMax = m_currPos.y;
				else
					rect->m_rect.yMin = m_currPos.y;

				m_captured.pos = m_currPos;
			}

			m_propertyPanel->EnablePropertyGrid(false);

			m_stage->GetCanvas()->SetDirty();
		}
	}

	return false;
}

bool EditRectOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) return true;

	if (m_captured.shape)
	{
		if (m_node_capture)
		{
			int tolerance = m_node_capture->GetValue();
			if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
			{
				d2d::Vector pos(rect->m_rect.xCenter(), rect->m_rect.yCenter());
				d2d::PrimitiveDraw::drawCircle(pos, tolerance, true, 2, d2d::Colorf(0.4f, 1.0f, 0.4f));
				if (m_captured.pos.isValid()) {
					d2d::PrimitiveDraw::drawCircle(m_captured.pos, tolerance, true, 2, d2d::Colorf(1.0f, 0.4f, 0.4f));
				}
			}
		}
	}
	else
	{
		if (m_firstPress.isValid() && m_currPos.isValid())
			d2d::PrimitiveDraw::rect(m_firstPress, m_currPos, m_style);
	}

	return false;
}

bool EditRectOP::Clear()
{
	if (d2d::ZoomViewOP::Clear()) return true;

	m_firstPress.setInvalid();
	m_currPos.setInvalid();

	return false;
}

}