#include "SetSpriteAngleAOP.h"
#include "Sprite.h"

#include <algorithm>

#include <assert.h>

namespace ee
{

SetSpriteAngleAOP::SetSpriteAngleAOP(Sprite* spr, float angle)
	: m_new_angle(angle)
{
	spr->AddReference();
	m_sprs.push_back(spr);
	m_old_angle.push_back(spr->GetAngle());
}

SetSpriteAngleAOP::SetSpriteAngleAOP(const std::vector<Sprite*>& sprs, float angle)
	: m_new_angle(angle)
{
	for_each(sprs.begin(), sprs.end(), cu::AddRefFonctor<Sprite>());
	m_sprs = sprs;

	for (int i = 0, n = sprs.size(); i < n; ++i) {
		m_old_angle.push_back(sprs[i]->GetAngle());
	}
}

SetSpriteAngleAOP::~SetSpriteAngleAOP()
{
	for_each(m_sprs.begin(), m_sprs.end(), cu::RemoveRefFonctor<Sprite>());
}

void SetSpriteAngleAOP::Undo()
{
	assert(m_sprs.size() == m_old_angle.size());
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		Sprite* spr = m_sprs[i];
		spr->SetAngle(m_old_angle[i]);
	}
}

void SetSpriteAngleAOP::Redo()
{
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		Sprite* spr = m_sprs[i];
		spr->SetAngle(m_new_angle);
	}
}

Json::Value SetSpriteAngleAOP::Store(const std::vector<Sprite*>& sprs) const
{
	Json::Value ret;
	return ret;
}

}