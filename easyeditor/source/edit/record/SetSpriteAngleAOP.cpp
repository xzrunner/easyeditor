#include "SetSpriteAngleAOP.h"
#include "Sprite.h"

#include <algorithm>

#include <assert.h>

namespace ee
{

SetSpriteAngleAOP::SetSpriteAngleAOP(const SprPtr& spr, float angle)
	: m_new_angle(angle)
{
	m_sprs.push_back(spr);
	m_old_angle.push_back(spr->GetAngle());
}

SetSpriteAngleAOP::SetSpriteAngleAOP(const std::vector<SprPtr>& sprs, float angle)
	: m_sprs(sprs)
	, m_new_angle(angle)
{
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		m_old_angle.push_back(sprs[i]->GetAngle());
	}
}

void SetSpriteAngleAOP::Undo()
{
	assert(m_sprs.size() == m_old_angle.size());
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetAngle(m_old_angle[i]);
	}
}

void SetSpriteAngleAOP::Redo()
{
	for (auto& spr : m_sprs) {
		spr->SetAngle(m_new_angle);
	}
}

Json::Value SetSpriteAngleAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value ret;
	return ret;
}

}