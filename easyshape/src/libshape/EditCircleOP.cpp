#include "EditCircleOP.h"
#include "CircleShape.h"
#include "CirclePropertySetting.h"

#include "INodeCapture.h"

namespace libshape
{

EditCircleOP::EditCircleOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl, 
						   d2d::PropertySettingPanel* propertyPanel, 
						   INodeCapture* node_capture)
	: ZoomViewOP(editPanel, true)
	, m_propertyPanel(propertyPanel)
	, m_shapesImpl(shapesImpl)
	, m_node_capture(node_capture)
{
	clear();
}

bool EditCircleOP::onKeyDown(int keyCode)
{
	if (ZoomViewOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapesImpl->removeShapeSelection();
		m_captured.clear();
		m_editPanel->Refresh();

		m_propertyPanel->setPropertySetting(NULL);
	}

	return false;
}

bool EditCircleOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_firstPress = m_currPos = m_editPanel->transPosScreenToProject(x, y);

	m_shapesImpl->getShapeSelection()->clear();

	int tolerance = m_node_capture ? m_node_capture->GetScope() : 0;
	if (tolerance != 0)
	{
		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_firstPress, m_captured);

		if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
		{
			m_propertyPanel->setPropertySetting(new CirclePropertySetting(m_editPanel, circle));
			m_shapesImpl->getShapeSelection()->insert(circle);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditCircleOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		if (m_firstPress.isValid())
		{
			m_currPos = m_editPanel->transPosScreenToProject(x, y);

			const float radius = d2d::Math::getDistance(m_firstPress, m_currPos);
			if (radius > 0)
			{
				CircleShape* circle = new CircleShape(m_firstPress, radius);
				m_propertyPanel->setPropertySetting(new CirclePropertySetting(m_editPanel, circle));
				m_shapesImpl->getShapeSelection()->insert(circle);
				m_shapesImpl->insertShape(circle);
			}
		}
	}
	else
	{
		m_propertyPanel->enablePropertyGrid(true);
		if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
			m_propertyPanel->setPropertySetting(new CirclePropertySetting(m_editPanel, circle));
	}

	clear();

	m_editPanel->Refresh();

	return false;
}

bool EditCircleOP::onMouseRightDown(int x, int y)
{
	if (ZoomViewOP::onMouseRightDown(x, y)) return true;

	int tolerance = m_node_capture ? m_node_capture->GetScope() : 0;
	if (tolerance != 0)
	{
		m_currPos = m_editPanel->transPosScreenToProject(x, y);

		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_currPos, m_captured);
		if (m_captured.shape)
		{
			m_shapesImpl->removeShape(m_captured.shape);
			m_shapesImpl->getShapeSelection()->clear();
			m_captured.clear();
			m_editPanel->Refresh();

			m_propertyPanel->setPropertySetting(NULL);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditCircleOP::onMouseMove(int x, int y)
{
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	int tolerance = m_node_capture ? m_node_capture->GetScope() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		d2d::IShape* old = m_captured.shape;
		capture.captureEditable(pos, m_captured);
		if (old && !m_captured.shape || !old && m_captured.shape)
			m_editPanel->Refresh();
	}

	return false;
}

bool EditCircleOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	m_currPos = m_editPanel->transPosScreenToProject(x, y);

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

			m_propertyPanel->enablePropertyGrid(false);
		}
	}

	m_editPanel->Refresh();

	return false;
}

bool EditCircleOP::onDraw(const d2d::Screen& scr) const
{
	if (ZoomViewOP::onDraw(scr)) return true;

	if (m_captured.shape)
	{
		if (m_node_capture)
		{
			int tolerance = m_node_capture->GetScope();
			if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
			{
				d2d::PrimitiveDraw::drawCircle(scr, circle->center, tolerance, 
					true, 2, d2d::Colorf(0.4f, 1.0f, 0.4f));
				if (!m_captured.pos.isValid()) {
					d2d::PrimitiveDraw::drawCircle(scr, circle->center, circle->radius,
						false, tolerance, d2d::Colorf(1.0f, 0.4f, 0.4f));
				}
			}
		}
	}
	else
	{
		if (m_firstPress.isValid() && m_currPos.isValid()) {
			d2d::PrimitiveDraw::drawCircle(scr, m_firstPress, d2d::Math::getDistance(m_firstPress, m_currPos), 
				false, 3, d2d::Colorf(0, 0, 0), 32);
		}
	}

	return false;
}

bool EditCircleOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_firstPress.setInvalid();
	m_currPos.setInvalid();

	return false;
}

}