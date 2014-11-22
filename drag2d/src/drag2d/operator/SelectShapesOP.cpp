#include "SelectShapesOP.h"

#include "common/visitors.h"
#include "common/Rect.h"
#include "view/MultiShapesImpl.h"
#include "view/PropertySettingPanel.h"
#include "component/AbstractEditCMPT.h"
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
	, m_bDraggable(true)
{
	m_selection = shapesImpl->getShapeSelection();
	m_selection->Retain();

	m_firstPos.setInvalid();
}

SelectShapesOP::~SelectShapesOP()
{
	clearClipboard();

 	m_selection->Clear();
 	m_selection->Release();
}

bool SelectShapesOP::onKeyDown(int keyCode)
{
	if (DrawRectangleOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapeImpl->removeShapeSelection();
		clear();
	}
	else if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_X))
	{
		clearClipboard();
		m_selection->Traverse(FetchAllVisitor<IShape>(m_clipboard));
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
			m_clipboard[i]->Retain();
		m_shapeImpl->removeShapeSelection();
	}
	else if (wxGetKeyState(WXK_CONTROL) && (keyCode == 'c' || keyCode == 'C'))
	{
		clearClipboard();

		std::vector<IShape*> shapes;
		m_selection->Traverse(FetchAllVisitor<IShape>(shapes));
		for (size_t i = 0, n = shapes.size(); i < n; ++i)
			m_clipboard.push_back(shapes[i]->clone());
	}
	else if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_V))
	{
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
		{
			m_shapeImpl->insertShape(m_clipboard[i]->clone());
			m_editPanel->Refresh();
		}
	}

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
			if (m_selection->IsExist(selected))
				m_selection->Remove(selected);
			else
			{
				m_selection->Add(selected);
				if (m_selection->Size() == 1)
					m_propertyPanel->setPropertySetting(createPropertySetting(selected));
				else
					m_propertyPanel->setPropertySetting(createPropertySetting(NULL));
			}
		}
		else
		{
			if (!m_selection->IsExist(selected))
			{
				m_selection->Clear();
				m_selection->Add(selected);
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
			m_selection->Clear();
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
			m_selection->Add(shapes[i]);

		if (m_propertyPanel)
		{
			if (m_selection->Size() == 1)
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

	m_selection->Traverse(DrawSelectedShapeVisitor());

	return false;
}

bool SelectShapesOP::clear()
{
	if (DrawRectangleOP::clear()) return true;

	clearClipboard();
	m_selection->Clear();
	m_firstPos.setInvalid();

	return false;
}

IPropertySetting* SelectShapesOP::createPropertySetting(IShape* shape) const
{
	return shape ? shape->createPropertySetting(m_editPanel) : NULL;
}

void SelectShapesOP::clearClipboard()
{
 	for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
 		m_clipboard[i]->Release();
 	m_clipboard.clear();
}

}