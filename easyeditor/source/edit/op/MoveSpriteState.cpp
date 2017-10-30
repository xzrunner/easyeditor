#include "MoveSpriteState.h"
#include "SpriteSelection.h"
#include "FetchAllVisitor.h"

#include <assert.h>

namespace ee
{

MoveSpriteState::MoveSpriteState(SpriteSelection* selection)
{
	m_center.Set(0, 0);
	selection->Traverse(FetchAllRefVisitor<Sprite>(m_sprs));
	if (!m_sprs.empty())
	{
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {
			m_center += m_sprs[i]->GetPosition();
		}
		m_center /= static_cast<float>(m_sprs.size());
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {
			m_offset.push_back(m_sprs[i]->GetPosition() - m_center);
		}
	}
}

bool MoveSpriteState::OnMouseMove(const sm::vec2& pos)
{
	if (m_sprs.empty()) {
		return false;
	}

	assert(m_sprs.size() == m_offset.size());
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetPosition(pos + m_offset[i]);
	}

	return true;
}

}