#include "SelectSpritesOP.h"

#include "common/Rect.h"
#include "dataset/TextSprite.h"
#include "dataset/FontSprite.h"
#include "dataset/Patch9Sprite.h"
#include "component/AbstractEditCMPT.h"
#include "view/PropertySettingPanel.h"
#include "view/SpritePropertySetting.h"
#include "view/MultiSpritesPropertySetting.h"
#include "view/TextPropertySetting.h"
#include "view/FontPropertySetting.h"
#include "view/Scale9PropertySetting.h"
#include "view/MultiSpritesImpl.h"
#include "render/DrawSelectedSpriteVisitor.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

SelectSpritesOP::SelectSpritesOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
								 PropertySettingPanel* propertyPanel, AbstractEditCMPT* callback/* = NULL*/)
	: DrawRectangleOP(editPanel)
	, m_callback(callback)
	, m_spritesImpl(spritesImpl)
	, m_propertyPanel(propertyPanel)
	, m_lastCtrlPress(false)
	, m_bDraggable(true)
{
	m_selection = spritesImpl->getSpriteSelection();
	m_selection->retain();

	m_firstPos.setInvalid();
}

SelectSpritesOP::~SelectSpritesOP()
{
	m_selection->clear();
	m_selection->release();
}

bool SelectSpritesOP::onKeyDown(int keyCode)
{
	if (DrawRectangleOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
		m_spritesImpl->removeSpriteSelection();	
	else if (wxGetKeyState(WXK_CONTROL_X))
	{
		clearClipboard();
		m_selection->traverse(FetchAllVisitor<ISprite>(m_clipboard));
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
			m_clipboard[i]->retain();
		m_spritesImpl->removeSpriteSelection();
	}
	else if (m_lastCtrlPress && (keyCode == 'c' || keyCode == 'C')/*wxGetKeyState(WXK_CONTROL_C)*/)
	{
		clearClipboard();

		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
			m_clipboard.push_back(sprites[i]->clone());
	}
	else if (wxGetKeyState(WXK_CONTROL_V))
	{
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
		{
			m_spritesImpl->insertSprite(m_clipboard[i]->clone());
			m_editPanel->Refresh();
		}
	}

	m_lastCtrlPress = keyCode == WXK_CONTROL;

	return false;
}

bool SelectSpritesOP::onMouseLeftDown(int x, int y)
{
	m_bDraggable = true;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (selected && selected->editable)
	{
		if (wxGetKeyState(WXK_CONTROL))
		{
			if (m_selection->isExist(selected))
				m_selection->erase(selected);
			else
			{
				m_selection->insert(selected);
				if (m_propertyPanel)
				{
					if (m_selection->size() == 1)
						m_propertyPanel->setPropertySetting(createPropertySetting(selected));
					else if (m_selection->size() > 1)
					{
						std::vector<ISprite*> sprites;
						m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
						m_propertyPanel->setPropertySetting(createPropertySetting(sprites));
					}
					else
						m_propertyPanel->setPropertySetting(createPropertySetting(NULL));
				}
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

bool SelectSpritesOP::onMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

	if (m_firstPos.isValid())
	{
		Vector end = m_editPanel->transPosScreenToProject(x, y);
		Rect rect(m_firstPos, end);
		std::vector<ISprite*> sprites;
		m_spritesImpl->querySpritesByRect(rect, m_firstPos.x < end.x, sprites);
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
			m_selection->insert(sprites[i]);

		if (m_propertyPanel)
		{
			if (m_selection->size() == 1)
				m_propertyPanel->setPropertySetting(createPropertySetting(sprites[0]));
			else if (m_selection->size() > 1)
				m_propertyPanel->setPropertySetting(createPropertySetting(sprites));
			else
				m_propertyPanel->setPropertySetting(createPropertySetting(NULL));
		}

		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}

//	enableRightTap(m_selection->empty());

	return false;
}

bool SelectSpritesOP::onMouseRightDown(int x, int y)
{
	m_rightFirstScrPos.set(x, y);

	enableRightTap(m_selection->empty());

	if (DrawRectangleOP::onMouseRightDown(x, y)) return true;

	return false;
}

bool SelectSpritesOP::onMouseRightUp(int x, int y)
{
	// select
	if (m_rightFirstScrPos == Vector(x, y))
	{
		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		d2d::ISprite* sprite = m_spritesImpl->querySpriteByPos(pos);
		if (sprite)
		{
			m_selection->clear();
			m_selection->insert(sprite);
			enableRightTap(m_selection->empty());
			m_editPanel->Refresh();
		}
	}

	if (DrawRectangleOP::onMouseRightUp(x, y)) return true;

	return false;
}

bool SelectSpritesOP::onMouseDrag(int x, int y)
{
	if (DrawRectangleOP::onMouseDrag(x, y)) return true;

	return !m_bDraggable;
}

bool SelectSpritesOP::onDraw() const
{
	m_selection->traverse(DrawSelectedSpriteVisitor(Colorf(1, 0, 0)));

	if (m_firstPos.isValid() && m_currPos.isValid())
	{
		if (m_currPos.x > m_firstPos.x)
		{
			m_style.lineStyle = LS_DEFAULT;

			m_style.fill = true;
			m_style.color.set(0, 0.22f, 0, 0.5f);
			PrimitiveDraw::rect(m_firstPos, m_currPos, m_style);

			m_style.fill = false;
			m_style.color.set(0.75f, 0, 0, 1);
			PrimitiveDraw::rect(m_firstPos, m_currPos, m_style);
		}
		else
		{
			m_style.lineStyle = LS_DASH;

			m_style.fill = true;
			m_style.color.set(0, 0, 0.22f, 0.5f);
			PrimitiveDraw::rect(m_firstPos, m_currPos, m_style);

			m_style.fill = false;
			m_style.color.set(0.75f, 0, 0, 1);
			PrimitiveDraw::rect(m_firstPos, m_currPos, m_style);
		}
	}

// 	if (DrawRectangleOP::onDraw()) return true;

	return false;
}

bool SelectSpritesOP::clear()
{
	if (DrawRectangleOP::clear()) return true;

	m_selection->clear();
	m_firstPos.setInvalid();

	return false;
}

IPropertySetting* SelectSpritesOP::createPropertySetting(ISprite* sprite) const
{
	if (TextSprite* text = dynamic_cast<TextSprite*>(sprite))
		return new TextPropertySetting(m_editPanel, text);
	else if (FontSprite* font = dynamic_cast<FontSprite*>(sprite))
		return new FontPropertySetting(m_editPanel, font);
//  	else if (Patch9Sprite* scale = dynamic_cast<Patch9Sprite*>(sprite))
//  		return new Scale9PropertySetting(m_editPanel, const_cast<Patch9Symbol*>(&scale->getSymbol()));
	else if (sprite)
		return new SpritePropertySetting(m_editPanel, sprite);
	else 
		return NULL;
}

IPropertySetting* SelectSpritesOP::createPropertySetting(const std::vector<ISprite*>& sprites) const
{
	return new MultiSpritesPropertySetting(m_editPanel, sprites);
}

void SelectSpritesOP::clearClipboard()
{
	for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
		m_clipboard[i]->release();
	m_clipboard.clear();
}

} // d2d