#include "SetSpritePosAOP.h"
#include "Sprite.h"

#include <algorithm>

#include <assert.h>

namespace ee
{

SetSpritePosAOP::SetSpritePosAOP(const SprPtr& spr, const sm::vec2& pos)
	: m_new_pos(pos)
{
	m_sprs.push_back(spr);
	m_old_pos.push_back(spr->GetPosition());
}

SetSpritePosAOP::SetSpritePosAOP(const std::vector<SprPtr>& sprs, const sm::vec2& pos)
	: m_new_pos(pos)
	, m_sprs(sprs)
{
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		m_old_pos.push_back(sprs[i]->GetPosition());
	}
}

void SetSpritePosAOP::Undo()
{
	assert(m_sprs.size() == m_old_pos.size());
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetPosition(m_old_pos[i]);
	}
}

void SetSpritePosAOP::Redo()
{
	for (auto& spr : m_sprs) {
		spr->SetPosition(m_new_pos);
	}
}

Json::Value SetSpritePosAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value ret;
	return ret;
}

}