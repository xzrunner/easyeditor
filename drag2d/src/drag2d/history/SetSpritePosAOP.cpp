#include "SetSpritePosAOP.h"

#include "dataset/Sprite.h"

#include <algorithm>

namespace d2d
{

SetSpritePosAOP::SetSpritePosAOP(Sprite* sprite, const Vector& pos)
	: m_new_pos(pos)
{
	sprite->Retain();
	m_sprites.push_back(sprite);
	m_old_pos.push_back(sprite->GetPosition());
}

SetSpritePosAOP::SetSpritePosAOP(const std::vector<Sprite*>& sprites, const Vector& pos)
	: m_new_pos(pos)
{
	for_each(sprites.begin(), sprites.end(), RetainObjectFunctor<Sprite>());
	m_sprites = sprites;

	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_old_pos.push_back(sprites[i]->GetPosition());
	}
}

SetSpritePosAOP::~SetSpritePosAOP()
{
	for_each(m_sprites.begin(), m_sprites.end(), ReleaseObjectFunctor<Sprite>());
}

void SetSpritePosAOP::Undo()
{
	assert(m_sprites.size() == m_old_pos.size());
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetTransform(m_old_pos[i], spr->GetAngle());
	}
}

void SetSpritePosAOP::Redo()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetTransform(m_new_pos, spr->GetAngle());
	}
}

Json::Value SetSpritePosAOP::Store(const std::vector<Sprite*>& sprites) const
{
	Json::Value ret;
	return ret;
}

}