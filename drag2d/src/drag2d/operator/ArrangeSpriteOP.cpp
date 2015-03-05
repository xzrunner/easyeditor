#ifndef D2D_ARRANGE_SPRITE_OP_CPP_
#define D2D_ARRANGE_SPRITE_OP_CPP_

#include "ArrangeSpriteOP.h"
#include "ArrangeSpriteImpl.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/AbstractBV.h"
#include "common/Math.h"
#include "common/visitors.h"
#include "common/Matrix.h"
#include "view/EditPanel.h"
#include "view/MultiSpritesImpl.h"
#include "view/PropertySettingPanel.h"
#include "history/ArrangeSpriteAtomicOP.h" 
#include "render/PrimitiveDraw.h"
//#include "render/DynamicTexture.h"
#include "render/DynamicTexAndFont.h"

namespace d2d
{

template <typename TBase>
ArrangeSpriteOP<TBase>::ArrangeSpriteOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
	PropertySettingPanel* propertyPanel/* = NULL*/, AbstractEditCMPT* callback/* = NULL*/, 
	const ArrangeSpriteConfig& cfg, ArrangeSpriteImpl* impl /*= NULL*/)
	: TBase(editPanel, spritesImpl, propertyPanel, callback)
{
	if (impl) {
		m_impl = impl;
	} else {
		m_impl = new ArrangeSpriteImpl(editPanel, spritesImpl, propertyPanel, cfg);
	}
}

template <typename TBase>
ArrangeSpriteOP<TBase>::~ArrangeSpriteOP()
{
	delete m_impl;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onKeyDown(int keyCode)
{
	if (TBase::onKeyDown(keyCode)) return true;
	m_impl->onKeyDown(keyCode);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onKeyUp(int keyCode)
{
	if (TBase::onKeyUp(keyCode)) return true;
	m_impl->onKeyUp(keyCode);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onMouseLeftDown(int x, int y)
{
	if (TBase::onMouseLeftDown(x, y)) return true;
	m_impl->onMouseLeftDown(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onMouseLeftUp(int x, int y)
{
	if (TBase::onMouseLeftUp(x, y)) return true;
	m_impl->onMouseLeftUp(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onMouseRightDown(int x, int y)
{
	if (TBase::onMouseRightDown(x, y)) return true;
	m_impl->onMouseRightDown(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onMouseRightUp(int x, int y)
{
	if (TBase::onMouseRightUp(x, y)) return true;
	m_impl->onMouseRightUp(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onMouseDrag(int x, int y)
{
	if (TBase::onMouseDrag(x, y)) return true;
	m_impl->onMouseDrag(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onPopMenuSelected(int type)
{
	if (TBase::onPopMenuSelected(type)) return true;
	m_impl->onPopMenuSelected(type);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onDraw() const
{
	if (TBase::onDraw()) return true;
	m_impl->onDraw(*m_editPanel->getCamera());
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::clear()
{
	if (TBase::clear()) return true;
	m_impl->clear();
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::IsEmpty() const
{
	return m_impl->IsSelectionEmpty();
}

template <typename TBase>
ISprite* ArrangeSpriteOP<TBase>::selectByPos(const Vector& pos) const
{
	ISprite* sprite = NULL;
	sprite = m_impl->QueryEditedSprite(pos);
	if (!sprite) {
		sprite = TBase::selectByPos(pos);
	}
	return sprite;
}

}

#endif // D2D_ARRANGE_SPRITE_OP_CPP_
