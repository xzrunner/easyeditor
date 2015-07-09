#include "MoveSpriteState.h"

#include "common/visitors.h"
#include "view/SpriteSelection.h"

namespace d2d
{

MoveSpriteState::MoveSpriteState(SpriteSelection* selection)
{
	m_center.set(0, 0);
	selection->Traverse(FetchAllVisitor<ISprite>(m_sprites));
	if (!m_sprites.empty())
	{
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			m_sprites[i]->Retain();
			m_center += m_sprites[i]->getPosition();
		}
		m_center /= m_sprites.size();
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			m_offset.push_back(m_sprites[i]->getPosition() - m_center);
		}
	}
}

MoveSpriteState::~MoveSpriteState()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
}

bool MoveSpriteState::OnMouseMove(const Vector& pos)
{
	if (m_sprites.empty()) {
		return false;
	}

	assert(m_sprites.size() == m_offset.size());
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* spr = m_sprites[i];
		spr->setTransform(pos + m_offset[i], spr->getAngle());
	}

	return true;
}

}