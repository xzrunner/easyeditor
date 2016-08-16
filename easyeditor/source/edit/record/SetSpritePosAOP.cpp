#include "SetSpritePosAOP.h"
#include "Sprite.h"

#include <algorithm>

#include <assert.h>

namespace ee
{

SetSpritePosAOP::SetSpritePosAOP(Sprite* sprite, const sm::vec2& pos)
	: m_new_pos(pos)
{
	sprite->AddReference();
	m_sprites.push_back(sprite);
	m_old_pos.push_back(sprite->GetPosition());
}

SetSpritePosAOP::SetSpritePosAOP(const std::vector<Sprite*>& sprites, const sm::vec2& pos)
	: m_new_pos(pos)
{
	for_each(sprites.begin(), sprites.end(), cu::AddRefFonctor<Sprite>());
	m_sprites = sprites;

	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_old_pos.push_back(sprites[i]->GetPosition());
	}
}

SetSpritePosAOP::~SetSpritePosAOP()
{
	for_each(m_sprites.begin(), m_sprites.end(), cu::RemoveRefFonctor<Sprite>());
}

void SetSpritePosAOP::Undo()
{
	assert(m_sprites.size() == m_old_pos.size());
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetPosition(m_old_pos[i]);
	}
}

void SetSpritePosAOP::Redo()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		Sprite* spr = m_sprites[i];
		spr->SetPosition(m_new_pos);
	}
}

Json::Value SetSpritePosAOP::Store(const std::vector<Sprite*>& sprites) const
{
	Json::Value ret;
	return ret;
}

}