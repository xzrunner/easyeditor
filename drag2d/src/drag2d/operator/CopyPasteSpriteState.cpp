#include "CopyPasteSpriteState.h"

#include "view/MultiSpritesImpl.h"

namespace d2d
{

CopyPasteSpriteState::CopyPasteSpriteState(MultiSpritesImpl* sprites_impl,
										   SpriteSelection* selection,
										   ISprite* selected)
	: m_selection(selection)
	, m_sprites_impl(sprites_impl)
	, m_spr(NULL)
{
	if (selected) {
		m_spr = selected->clone();
		sprites_impl->InsertSprite(m_spr);
		m_selection->Clear();
		m_selection->Add(m_spr);
	}
}

CopyPasteSpriteState::~CopyPasteSpriteState()
{
	if (m_spr) {
		m_spr->Release();
	}
}

bool CopyPasteSpriteState::OnMouseDrag(const Vector& pos)
{
	m_spr->setTransform(pos, m_spr->getAngle());
	return true;
}

}