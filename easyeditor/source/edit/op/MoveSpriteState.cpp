#include "MoveSpriteState.h"
#include "SpriteSelection.h"
#include "FetchAllVisitor.h"

#include <assert.h>

namespace ee
{

MoveSpriteState::MoveSpriteState(SpriteSelection* selection)
{
	m_center.Set(0, 0);
	selection->Traverse(FetchAllVisitor<Sprite>(m_sprites));
	if (!m_sprites.empty())
	{
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			m_sprites[i]->AddReference();
			m_center += m_sprites[i]->GetPosition();
		}
		m_center /= static_cast<float>(m_sprites.size());
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			m_offset.push_back(m_sprites[i]->GetPosition() - m_center);
		}
	}
}

MoveSpriteState::~MoveSpriteState()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->RemoveReference();
	}
}

bool MoveSpriteState::OnMouseMove(const sm::vec2& pos)
{
	if (m_sprites.empty()) {
		return false;
	}

	assert(m_sprites.size() == m_offset.size());
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetPosition(pos + m_offset[i]);
	}

	return true;
}

}