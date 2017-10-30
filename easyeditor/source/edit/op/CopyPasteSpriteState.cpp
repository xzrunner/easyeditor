#include "CopyPasteSpriteState.h"
#include "Sprite.h"
#include "SpriteSelection.h"
#include "sprite_msg.h"
#include "FetchAllVisitor.h"
#include "SpriteFactory.h"

#include <algorithm>

namespace ee
{

CopyPasteSpriteState::CopyPasteSpriteState(SpriteSelection* selection)
{
	std::vector<SprPtr> sprs;
	selection->Traverse(FetchAllRefVisitor<Sprite>(sprs));

	m_sprs.reserve(sprs.size());
	selection->Clear();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		auto spr = std::dynamic_pointer_cast<ee::Sprite>(sprs[i]->Clone());
		spr->SetName(SpriteFactory::Instance()->NextName().c_str());
		m_sprs.push_back(spr);
		selection->Add(spr);
	}
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		InsertSpriteSJ::Instance()->Insert(m_sprs[i]);
	}
}

void CopyPasteSpriteState::OnMousePress(const sm::vec2& pos)
{
	m_last_pos = pos;
}

bool CopyPasteSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	sm::vec2 offset = pos - m_last_pos;
	for (auto& spr : m_sprs) {
		spr->Translate(offset);
	}
	m_last_pos = pos;
	return true;
}

}