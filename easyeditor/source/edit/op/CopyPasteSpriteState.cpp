#include "CopyPasteSpriteState.h"
#include "Sprite.h"
#include "SpriteSelection.h"
#include "sprite_msg.h"
#include "FetchAllVisitor.h"

#include <algorithm>

namespace ee
{

CopyPasteSpriteState::CopyPasteSpriteState(SpriteSelection* selection)
{
	std::vector<Sprite*> sprs;
	selection->Traverse(FetchAllVisitor<Sprite>(sprs));

	m_sprs.reserve(sprs.size());
	selection->Clear();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		Sprite* spr = dynamic_cast<Sprite*>(((cu::Cloneable*)sprs[i])->Clone());
		m_sprs.push_back(spr);
		selection->Add(spr);
	}
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		InsertSpriteSJ::Instance()->Insert(m_sprs[i]);
	}
}

CopyPasteSpriteState::~CopyPasteSpriteState()
{
	for_each(m_sprs.begin(), m_sprs.end(), cu::RemoveRefFonctor<Sprite>());
}

void CopyPasteSpriteState::OnMousePress(const sm::vec2& pos)
{
	m_last_pos = pos;
}

bool CopyPasteSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	sm::vec2 offset = pos - m_last_pos;
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		Sprite* spr = m_sprs[i];
		spr->Translate(offset);
	}
	m_last_pos = pos;
	return true;
}

}