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
	PropertySettingPanel* propertyPanel/* = NULL*/, ViewPanelMgr* view_panel_mgr /*= NULL*/, 
	AbstractEditCMPT* callback/* = NULL*/, const ArrangeSpriteConfig& cfg, ArrangeSpriteImpl* impl /*= NULL*/)
	: TBase(editPanel, spritesImpl, view_panel_mgr, callback)
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
bool ArrangeSpriteOP<TBase>::OnKeyDown(int keyCode)
{
	if (TBase::OnKeyDown(keyCode)) return true;
	m_impl->OnKeyDown(keyCode);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnKeyUp(int keyCode)
{
	if (TBase::OnKeyUp(keyCode)) return true;
	m_impl->OnKeyUp(keyCode);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseLeftDown(int x, int y)
{
	if (TBase::OnMouseLeftDown(x, y)) return true;
	m_impl->OnMouseLeftDown(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseLeftUp(int x, int y)
{
	if (TBase::OnMouseLeftUp(x, y)) return true;
	m_impl->OnMouseLeftUp(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseRightDown(int x, int y)
{
	if (TBase::OnMouseRightDown(x, y)) return true;
	m_impl->OnMouseRightDown(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseRightUp(int x, int y)
{
	if (TBase::OnMouseRightUp(x, y)) return true;
	m_impl->OnMouseRightUp(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseMove(int x, int y)
{
	if (TBase::OnMouseDrag(x, y)) return true;
	m_impl->OnMouseMove(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseDrag(int x, int y)
{
	if (TBase::OnMouseDrag(x, y)) return true;
	m_impl->OnMouseDrag(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnPopMenuSelected(int type)
{
	if (TBase::OnPopMenuSelected(type)) return true;
	m_impl->OnPopMenuSelected(type);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnDraw() const
{
	if (TBase::OnDraw()) return true;
	m_impl->OnDraw(*m_stage->getCamera());
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::Clear()
{
	if (TBase::Clear()) return true;
	m_impl->Clear();
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::IsEmpty() const
{
	return m_impl->IsSelectionEmpty();
}

template <typename TBase>
ISprite* ArrangeSpriteOP<TBase>::SelectByPos(const Vector& pos) const
{
	ISprite* sprite = NULL;
	sprite = m_impl->QueryEditedSprite(pos);
	if (!sprite) {
		sprite = TBase::SelectByPos(pos);
	}
	return sprite;
}

}

#endif // D2D_ARRANGE_SPRITE_OP_CPP_
