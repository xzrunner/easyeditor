#include "SetSpriteAngleAOP.h"

#include "dataset/ISprite.h"

#include <algorithm>

namespace d2d
{

SetSpriteAngleAOP::SetSpriteAngleAOP(ISprite* sprite, float angle)
	: m_new_angle(angle)
{
	sprite->Retain();
	m_sprites.push_back(sprite);
	m_old_angle.push_back(sprite->GetAngle());
}

SetSpriteAngleAOP::SetSpriteAngleAOP(const std::vector<ISprite*>& sprites, float angle)
	: m_new_angle(angle)
{
	for_each(sprites.begin(), sprites.end(), RetainObjectFunctor<ISprite>());
	m_sprites = sprites;

	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_old_angle.push_back(sprites[i]->GetAngle());
	}
}

SetSpriteAngleAOP::~SetSpriteAngleAOP()
{
	for_each(m_sprites.begin(), m_sprites.end(), ReleaseObjectFunctor<ISprite>());
}

void SetSpriteAngleAOP::Undo()
{
	assert(m_sprites.size() == m_old_angle.size());
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* spr = m_sprites[i];
		spr->SetTransform(spr->GetPosition(), m_old_angle[i]);
	}
}

void SetSpriteAngleAOP::Redo()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* spr = m_sprites[i];
		spr->SetTransform(spr->GetPosition(), m_new_angle);
	}
}

Json::Value SetSpriteAngleAOP::Store(const std::vector<ISprite*>& sprites) const
{
	Json::Value ret;
	return ret;
}

}