#include "CopyPasteSpriteState.h"

#include "dataset/ISprite.h"
#include "view/SpriteSelection.h"
#include "message/InsertSpriteSJ.h"

namespace d2d
{

CopyPasteSpriteState::CopyPasteSpriteState(SpriteSelection* selection, ISprite* selected)
	: m_selection(selection)
	, m_spr(NULL)
{
	if (selected) {
		m_spr = selected->Clone();
		InsertSpriteSJ::Instance()->Insert(m_spr);
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
	m_spr->SetTransform(pos, m_spr->GetAngle());
	return true;
}

}