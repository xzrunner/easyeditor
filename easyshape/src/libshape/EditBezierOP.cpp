#include "EditBezierOP.h"
#include "BezierShape.h"
#include "BezierPropertySetting.h"

namespace libshape
{

EditBezierOP::EditBezierOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
						   d2d::PropertySettingPanel* propertyPanel, 
						   d2d::ViewPanelMgr* view_panel_mgr,
						   d2d::OneFloatValue* node_capture)
	: ZoomViewOP(editPanel, true)
	, m_propertyPanel(propertyPanel)
	, m_view_panel_mgr(view_panel_mgr)
	, m_shapesImpl(shapesImpl)
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
		m_shapesImpl->ClearShapeSelection();
		m_captured.clear();
		m_stage->Refresh();

		if (m_view_panel_mgr) {
			m_view_panel_mgr->SelectShape(NULL, m_shapesImpl);
		}
	}

	return false;
}

bool EditBezierOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_firstPress = m_currPos = m_stage->TransPosScrToProj(x, y);

	m_shapesImpl->GetShapeSelection()->Clear();

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_firstPress, m_captured);
 		if (BezierShape* bezier = dynamic_cast<BezierShape*>(m_captured.shape))
		{
			m_shapesImpl->GetShapeSelection()->Add(bezier);
			if (m_view_panel_mgr) {
				m_view_panel_mgr->SelectShape(bezier, m_shapesImpl);
			}
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
		if (m_firstPress.isValid())
		{
			m_currPos = m_stage->TransPosScrToProj(x, y);

			const float dis = d2d::Math::getDistance(m_firstPress, m_currPos);
			if (dis > 1)
			{
				BezierShape* bezier = new BezierShape(m_firstPress, m_currPos);
				if (m_view_panel_mgr) {
					m_view_panel_mgr->SelectShape(bezier, m_shapesImpl);
				}
				m_shapesImpl->GetShapeSelection()->Add(bezier);
				m_shapesImpl->InsertShape(bezier);
			}
		}
	}
	else
	{
 		m_propertyPanel->EnablePropertyGrid(true);
		if (m_view_panel_mgr) {
			m_view_panel_mgr->SelectShape(m_captured.shape, m_shapesImpl);
		}
	}

	Clear();

	m_stage->Refresh();

	return false;
}

bool EditBezierOP::OnMouseRightDown(int x, int y)
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
			m_stage->Refresh();

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

bool EditBezierOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		d2d::IShape* old = m_captured.shape;
		capture.captureEditable(pos, m_captured);
		if (old && !m_captured.shape || !old && m_captured.shape)
			m_stage->Refresh();
	}

	return false;
}

bool EditBezierOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_currPos = m_stage->TransPosScrToProj(x, y);

	if (m_captured.shape)
	{
		if (BezierShape* bezier = dynamic_cast<BezierShape*>(m_captured.shape))
		{
			d2d::Vector center(bezier->getRect().xCenter(), bezier->getRect().yCenter());

			if (!m_captured.pos.isValid()) {
				bezier->Translate(m_currPos - center);
			} else {
				bezier->MoveCtrlNode(m_captured.pos, m_currPos);
				m_captured.pos = m_currPos;
			}

			m_propertyPanel->EnablePropertyGrid(false);
		}
	}

	m_stage->Refresh();

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
				d2d::PrimitiveDraw::drawCircle(d2d::Vector(bezier->getRect().xCenter(), bezier->getRect().yCenter()), 
					tolerance, true, 2, d2d::Colorf(0.4f, 1.0f, 0.4f));
				if (m_captured.pos.isValid()) {
					d2d::PrimitiveDraw::drawCircle(m_captured.pos, tolerance, true, 2, d2d::Colorf(1.0f, 0.4f, 0.4f));
				}
			}
		}
	}
	else
	{
		if (m_firstPress.isValid() && m_currPos.isValid())
		{
			BezierShape bezier(m_firstPress, m_currPos);
			bezier.draw(d2d::Matrix());
		}
//			PrimitiveDraw::drawRect(m_firstPress, m_currPos);
	}

	return false;
}

bool EditBezierOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_firstPress.setInvalid();
	m_currPos.setInvalid();

	return false;
}

}