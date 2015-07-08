#include "DeleteSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"

#include "dataset/ISprite.h"
#include "view/MultiSpritesImpl.h"

namespace d2d
{

DeleteSpriteAOP::DeleteSpriteAOP(const std::vector<ISprite*>& sprites, 
								 MultiSpritesImpl* sprites_impl)
	: m_sprites_impl(sprites_impl)
{
	for (size_t i = 0, n = sprites.size(); i < n; ++i) 
	{
		sprites[i]->Retain();
		m_sprites.push_back(sprites[i]);
	}
}

DeleteSpriteAOP::~DeleteSpriteAOP()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
}

void DeleteSpriteAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites_impl->InsertSprite(m_sprites[i]);
	}
}

void DeleteSpriteAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites_impl->RemoveSprite(m_sprites[i]);
	}
}

Json::Value DeleteSpriteAOP::store(const std::vector<ISprite*>& sprites)
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprites, sprites);
	val["type"] = AT_DELETE;
	return val;
}

}