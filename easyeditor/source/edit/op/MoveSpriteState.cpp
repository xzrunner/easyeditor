#include "MoveSpriteState.h"
#include "SpriteSelection.h"
#include "FetchAllVisitor.h"

#include <assert.h>

namespace ee
{

MoveSpriteState::MoveSpriteState(SpriteSelection* selection)
{
	m_center.Set(0, 0);
	selection->Traverse(FetchAllVisitor<Sprite>(m_sprs));
	if (!m_sprs.empty())
	{
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {
			m_sprs[i]->AddReference();
			m_center += m_sprs[i]->GetPosition();
		}
		m_center /= static_cast<float>(m_sprs.size());
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {
			m_offset.push_back(m_sprs[i]->GetPosition() - m_center);
		}
	}
}

MoveSpriteState::~MoveSpriteState()
{
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->RemoveReference();
	}
}

bool MoveSpriteState::OnMouseMove(const sm::vec2& pos)
{
	if (m_sprs.empty()) {
		return false;
	}

	assert(m_sprs.size() == m_offset.size());
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		Sprite* spr = m_sprs[i];
		spr->SetPosition(pos + m_offset[i]);
	}

	return true;
}

}