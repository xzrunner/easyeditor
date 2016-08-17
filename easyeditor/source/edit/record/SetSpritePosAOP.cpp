#include "SetSpritePosAOP.h"
#include "Sprite.h"

#include <algorithm>

#include <assert.h>

namespace ee
{

SetSpritePosAOP::SetSpritePosAOP(Sprite* spr, const sm::vec2& pos)
	: m_new_pos(pos)
{
	spr->AddReference();
	m_sprs.push_back(spr);
	m_old_pos.push_back(spr->GetPosition());
}

SetSpritePosAOP::SetSpritePosAOP(const std::vector<Sprite*>& sprs, const sm::vec2& pos)
	: m_new_pos(pos)
{
	for_each(sprs.begin(), sprs.end(), cu::AddRefFonctor<Sprite>());
	m_sprs = sprs;

	for (int i = 0, n = sprs.size(); i < n; ++i) {
		m_old_pos.push_back(sprs[i]->GetPosition());
	}
}

SetSpritePosAOP::~SetSpritePosAOP()
{
	for_each(m_sprs.begin(), m_sprs.end(), cu::RemoveRefFonctor<Sprite>());
}

void SetSpritePosAOP::Undo()
{
	assert(m_sprs.size() == m_old_pos.size());
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		Sprite* spr = m_sprs[i];
		spr->SetPosition(m_old_pos[i]);
	}
}

void SetSpritePosAOP::Redo()
{
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		Sprite* spr = m_sprs[i];
		spr->SetPosition(m_new_pos);
	}
}

Json::Value SetSpritePosAOP::Store(const std::vector<Sprite*>& sprs) const
{
	Json::Value ret;
	return ret;
}

}