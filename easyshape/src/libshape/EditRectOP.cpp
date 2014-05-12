#include "EditRectOP.h"
#include "RectShape.h"
#include "RectPropertySetting.h"

namespace libshape
{

EditRectOP::EditRectOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
					   d2d::PropertySettingPanel* propertyPanel,
					   NodeCaptureCMPT<EditRectOP>* cmpt)
	: d2d::ZoomViewOP(editPanel, true)
	, m_propertyPanel(propertyPanel)
	, m_shapesImpl(shapesImpl)
	, m_cmpt(cmpt)
{
	clear();
}

bool EditRectOP::onKeyDown(int keyCode)
{
	if (d2d::ZoomViewOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapesImpl->removeShapeSelection();
		m_captured.clear();
		m_editPanel->Refresh();

		m_propertyPanel->setPropertySetting(NULL);
	}

	return false;
}

bool EditRectOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

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

		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
		{
			m_propertyPanel->setPropertySetting(new RectPropertySetting(m_editPanel, rect));
			m_shapesImpl->getShapeSelection()->insert(rect);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditRectOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		if (m_firstPress.isValid())
		{
			m_currPos = m_editPanel->transPosScreenToProject(x, y);

			const float dis = d2d::Math::getDistance(m_firstPress, m_currPos);
			if (dis > 1)
			{
				RectShape* rect = new RectShape(m_firstPress, m_currPos);
				m_propertyPanel->setPropertySetting(new RectPropertySetting(m_editPanel, rect));
				m_shapesImpl->getShapeSelection()->insert(rect);
				m_shapesImpl->insertShape(rect);
			}
		}
	}
	else
	{
		m_propertyPanel->enablePropertyGrid(true);
		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
			m_propertyPanel->setPropertySetting(new RectPropertySetting(m_editPanel, rect));
	}

	clear();

	m_editPanel->Refresh();

	return false;
}

bool EditRectOP::onMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseRightDown(x, y)) return true;

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

bool EditRectOP::onMouseMove(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseMove(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
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

bool EditRectOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y)) return true;

	m_currPos = m_editPanel->transPosScreenToProject(x, y);

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

			m_propertyPanel->enablePropertyGrid(false);
		}
	}

	m_editPanel->Refresh();

	return false;
}

bool EditRectOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw()) return true;

	if (m_captured.shape)
	{
		if (m_cmpt)
		{
			if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
			{
				d2d::PrimitiveDraw::drawCircle(d2d::Vector(rect->m_rect.xCenter(), rect->m_rect.yCenter()), 
					m_cmpt->getNodeCaptureDistance(), true, 2, d2d::Colorf(0.4f, 1.0f, 0.4f));
				if (m_captured.pos.isValid())
					d2d::PrimitiveDraw::drawCircle(m_captured.pos, m_cmpt->getNodeCaptureDistance(), 
						true, 2, d2d::Colorf(1.0f, 0.4f, 0.4f));
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

bool EditRectOP::clear()
{
	if (d2d::ZoomViewOP::clear()) return true;

	m_firstPress.setInvalid();
	m_currPos.setInvalid();

	return false;
}

}