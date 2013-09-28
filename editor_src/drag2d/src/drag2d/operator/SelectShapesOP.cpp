#include "SelectShapesOP.h"

#include "common/visitors.h"
#include "dataset/BezierShape.h"
#include "dataset/PolygonShape.h"
#include "dataset/CircleShape.h"
#include "dataset/RectShape.h"
#include "component/AbstractEditCMPT.h"
#include "view/PropertySettingPanel.h"
#include "view/BezierPropertySetting.h"
#include "view/PolygonPropertySetting.h"
#include "view/ChainPropertySetting.h"
#include "view/CirclePropertySetting.h"
#include "view/RectPropertySetting.h"
#include "view/MultiShapesImpl.h"
#include "render/DrawSelectedShapeVisitor.h"

namespace d2d
{

SelectShapesOP::SelectShapesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
							   PropertySettingPanel* propertyPanel, 
							   AbstractEditCMPT* callback/* = NULL*/)
	: DrawRectangleOP(editPanel)
	, m_callback(callback)
	, m_shapeImpl(shapesImpl)
	, m_propertyPanel(propertyPanel)
	, m_lastCtrlPress(false)
	, m_bDraggable(true)
{
	m_selection = shapesImpl->getShapeSelection();
	m_selection->retain();

	m_firstPos.setInvalid();
}

SelectShapesOP::~SelectShapesOP()
{
	clearClipboard();

 	m_selection->clear();
 	m_selection->release();
}

bool SelectShapesOP::onKeyDown(int keyCode)
{
	if (DrawRectangleOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapeImpl->removeShapeSelection();
		clear();
	}
	else if (wxGetKeyState(WXK_CONTROL_X))
	{
		clearClipboard();
		m_selection->traverse(FetchAllVisitor<IShape>(m_clipboard));
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
			m_clipboard[i]->retain();
		m_shapeImpl->removeShapeSelection();
	}
	else if (m_lastCtrlPress && (keyCode == 'c' || keyCode == 'C')/*wxGetKeyState(WXK_CONTROL_C)*/)
	{
		clearClipboard();

		std::vector<PolygonShape*> polys;
		m_selection->traverse(FetchAllVisitor<PolygonShape>(polys));
		for (size_t i = 0, n = polys.size(); i < n; ++i)
			m_clipboard.push_back(polys[i]->clone());
	}
	else if (wxGetKeyState(WXK_CONTROL_V))
	{
		// todo: should deep copy
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
		{
			m_clipboard[i]->retain();
			m_shapeImpl->insertShape(m_clipboard[i]);
			m_editPanel->Refresh();
		}
	}

	m_lastCtrlPress = keyCode == WXK_CONTROL;

	return false;
}

bool SelectShapesOP::onMouseLeftDown(int x, int y)
{
	m_bDraggable = true;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	IShape* selected = m_shapeImpl->queryShapeByPos(pos);
	if (selected)
	{
		if (wxGetKeyState(WXK_CONTROL))
		{
			if (m_selection->isExist(selected))
				m_selection->erase(selected);
			else
			{
				m_selection->insert(selected);
				if (m_selection->size() == 1)
					m_propertyPanel->setPropertySetting(createPropertySetting(selected));
				else
					m_propertyPanel->setPropertySetting(createPropertySetting(NULL));
			}
		}
		else
		{
			if (!m_selection->isExist(selected))
			{
				m_selection->clear();
				m_selection->insert(selected);
				if (m_propertyPanel)
					m_propertyPanel->setPropertySetting(createPropertySetting(selected));
			}
		}
		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}
	else
	{
		DrawRectangleOP::onMouseLeftDown(x, y);
		m_firstPos = pos;
		if (wxGetKeyState(WXK_CONTROL))
			m_bDraggable = false;
		else
			m_selection->clear();
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectShapesOP::onMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

	if (m_firstPos.isValid())
	{
		Rect rect(m_firstPos, m_editPanel->transPosScreenToProject(x, y));
		std::vector<IShape*> shapes;
		m_shapeImpl->queryShapesByRect(rect, shapes);
		for (size_t i = 0, n = shapes.size(); i < n; ++i)
			m_selection->insert(shapes[i]);

		if (m_propertyPanel)
		{
			if (m_selection->size() == 1)
				m_propertyPanel->setPropertySetting(createPropertySetting(shapes[0]));
			else
				m_propertyPanel->setPropertySetting(createPropertySetting(NULL));
		}

		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}

	return false;
}

bool SelectShapesOP::onMouseDrag(int x, int y)
{
	if (DrawRectangleOP::onMouseDrag(x, y)) return true;

	return !m_bDraggable;
}

bool SelectShapesOP::onDraw() const
{
	if (DrawRectangleOP::onDraw()) return true;

	m_selection->traverse(DrawSelectedShapeVisitor());

	return false;
}

bool SelectShapesOP::clear()
{
	if (DrawRectangleOP::clear()) return true;

	clearClipboard();
	m_selection->clear();
	m_firstPos.setInvalid();

	return false;
}

IPropertySetting* SelectShapesOP::createPropertySetting(IShape* shape) const
{
	if (BezierShape* bezier = dynamic_cast<BezierShape*>(shape))
		return new BezierPropertySetting(m_editPanel, bezier);
	else if (PolygonShape* polygon = dynamic_cast<PolygonShape*>(shape))
		return new PolygonPropertySetting(m_editPanel, polygon);
	else if (ChainShape* chain = dynamic_cast<ChainShape*>(shape))
		return new ChainPropertySetting(m_editPanel, chain);
	else if (CircleShape* circle = dynamic_cast<CircleShape*>(shape))
		return new CirclePropertySetting(m_editPanel, circle);
	else if (RectShape* rect = dynamic_cast<RectShape*>(shape))
		return new RectPropertySetting(m_editPanel, rect);
	else
		return NULL;
}

void SelectShapesOP::clearClipboard()
{
 	for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
 		m_clipboard[i]->release();
 	m_clipboard.clear();
}

} // d2d