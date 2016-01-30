#ifndef D2D_ARRANGE_SPRITE_OP_CPP_
#define D2D_ARRANGE_SPRITE_OP_CPP_

#include "ArrangeSpriteOP.h"
#include "ArrangeSpriteImpl.h"

#include "dataset/Sprite.h"
#include "dataset/Symbol.h"
#include "dataset/BoundingBox.h"
#include "common/Math.h"
#include "common/visitors.h"
#include "common/Matrix.h"
#include "view/EditPanelImpl.h"
#include "view/MultiSpritesImpl.h"
#include "view/PropertySettingPanel.h"
#include "history/ArrangeSpriteAtomicOP.h" 
#include "render/PrimitiveDraw.h"

namespace d2d
{

template <typename TBase>
ArrangeSpriteOP<TBase>::ArrangeSpriteOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* spritesImpl, 
	PropertySettingPanel* propertyPanel/* = NULL*/, AbstractEditCMPT* callback/* = NULL*/, 
	const ArrangeSpriteConfig& cfg, ArrangeSpriteImpl* impl /*= NULL*/)
	: TBase(wnd, stage, spritesImpl, callback)
{
	if (impl) {
		m_impl = impl;
	} else {
		m_impl = new ArrangeSpriteImpl(wnd, stage, spritesImpl, propertyPanel, cfg);
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
	if (TBase::OnMouseMove(x, y)) return true;
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
	m_impl->OnDraw(*m_stage->GetCamera());
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
Sprite* ArrangeSpriteOP<TBase>::SelectByPos(const Vector& pos) const
{
	Sprite* sprite = NULL;
	sprite = m_impl->QueryEditedSprite(pos);
	if (!sprite) {
		sprite = TBase::SelectByPos(pos);
	}
	return sprite;
}

}

#endif // D2D_ARRANGE_SPRITE_OP_CPP_
