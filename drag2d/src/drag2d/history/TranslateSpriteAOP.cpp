#include "TranslateSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"

#include "common/visitors.h"
#include "view/SpriteSelection.h"

#include <algorithm>

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
	for_each(sprites.begin(), sprites.end(), RetainObjectFunctor<ISprite>());
	m_sprites = sprites;
}

TranslateSpriteAOP::~TranslateSpriteAOP()
{
	for_each(m_sprites.begin(), m_sprites.end(), ReleaseObjectFunctor<ISprite>());
}

void TranslateSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* sprite = m_sprites[i];
		sprite->Translate(-m_offset);
	}
}

void TranslateSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* sprite = m_sprites[i];
		sprite->Translate(m_offset);
	}
}

Json::Value TranslateSpriteAOP::Store(const std::vector<ISprite*>& sprites) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprites, sprites);
	val["type"] = AT_MOVE;
	val["dx"] = m_offset.x;
	val["dy"] = m_offset.y;
	return val;
}

}