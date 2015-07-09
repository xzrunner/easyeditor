#include "TranslateSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"

#include "common/visitors.h"
#include "view/SpriteSelection.h"

namespace d2d
{

TranslateSpriteAOP::TranslateSpriteAOP(const SpriteSelection& selection, const Vector& offset)
	: m_offset(offset)
{
	selection.Traverse(FetchAllVisitor<ISprite>(m_sprites));
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Retain();
	}
}

TranslateSpriteAOP::TranslateSpriteAOP(ISprite* sprite, const Vector& offset)
	: m_offset(offset)
{
	sprite->Retain();
	m_sprites.push_back(sprite);
}

TranslateSpriteAOP::TranslateSpriteAOP(const std::vector<ISprite*>& sprites, const Vector& offset)
	: m_offset(offset)
{
	for (size_t i = 0, n = sprites.size(); i < n; ++i) 
	{
		sprites[i]->Retain();
		m_sprites.push_back(sprites[i]);
	}
}

TranslateSpriteAOP::~TranslateSpriteAOP()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
}

void TranslateSpriteAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* sprite = m_sprites[i];
		sprite->Translate(-m_offset);
	}
}

void TranslateSpriteAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* sprite = m_sprites[i];
		sprite->Translate(m_offset);
	}
}

Json::Value TranslateSpriteAOP::store(const std::vector<ISprite*>& sprites)
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprites, sprites);
	val["type"] = AT_MOVE;
	val["dx"] = m_offset.x;
	val["dy"] = m_offset.y;
	return val;
}

}