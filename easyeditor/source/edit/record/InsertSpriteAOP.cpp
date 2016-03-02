#include "InsertSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "Sprite.h"
#include "MultiSpritesImpl.h"
#include "sprite_msg.h"

#include <algorithm>

namespace ee
{

InsertSpriteAOP::InsertSpriteAOP(Sprite* spr)
{
	spr->Retain();
	m_sprites.push_back(spr);
}

InsertSpriteAOP::InsertSpriteAOP(const std::vector<Sprite*>& sprites)
{	
	for_each(sprites.begin(), sprites.end(), RetainObjectFunctor<Sprite>());
	m_sprites = sprites;
}

InsertSpriteAOP::~InsertSpriteAOP()
{
	for_each(m_sprites.begin(), m_sprites.end(), ReleaseObjectFunctor<Sprite>());
}

void InsertSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(m_sprites[i]);
	}
}

void InsertSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		InsertSpriteSJ::Instance()->Insert(m_sprites[i]);
	}
}

Json::Value InsertSpriteAOP::Store(const std::vector<Sprite*>& sprites) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprites, sprites);
	val["type"] = AT_INSERT;
	return val;
}

}