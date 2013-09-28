#include "EditCircleOP.h"

#include "common/Math.h"
#include "view/MultiShapesImpl.h"
#include "view/PropertySettingPanel.h"
#include "view/CirclePropertySetting.h"
#include "dataset/CircleShape.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

EditCircleOP::EditCircleOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl, 
						   PropertySettingPanel* propertyPanel,
						   NodeCaptureCMPT<EditCircleOP>* cmpt)
	: ZoomViewOP(editPanel, true)
	, m_propertyPanel(propertyPanel)
	, m_shapesImpl(shapesImpl)
	, m_cmpt(cmpt)
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

	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
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

			const float radius = Math::getDistance(m_firstPress, m_currPos);
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

	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
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

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		IShape* old = m_captured.shape;
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
				circle->radius = Math::getDistance(m_currPos, circle->center);

			m_propertyPanel->enablePropertyGrid(false);
		}
	}

	m_editPanel->Refresh();

	return false;
}

bool EditCircleOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	if (m_captured.shape)
	{
		if (m_cmpt)
		{
			if (CircleShape* circle = dynamic_cast<CircleShape*>(m_captured.shape))
			{
				PrimitiveDraw::drawCircle(circle->center, m_cmpt->getNodeCaptureDistance(), 
					true, 2, Colorf(0.4f, 1.0f, 0.4f));
				if (!m_captured.pos.isValid())
					PrimitiveDraw::drawCircle(circle->center, circle->radius,
						false, m_cmpt->getNodeCaptureDistance(), Colorf(1.0f, 0.4f, 0.4f));
			}
		}
	}
	else
	{
		if (m_firstPress.isValid() && m_currPos.isValid())
			PrimitiveDraw::drawCircle(m_firstPress, Math::getDistance(m_firstPress, m_currPos), false, 3, Colorf(0, 0, 0), 32);
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
} // d2d