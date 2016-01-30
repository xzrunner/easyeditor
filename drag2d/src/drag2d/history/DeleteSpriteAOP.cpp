#include "DeleteSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"

#include "dataset/Sprite.h"
#include "view/MultiSpritesImpl.h"

#include "message/InsertSpriteSJ.h"
#include "message/RemoveSpriteSJ.h"

namespace d2d
{

DeleteSpriteAOP::DeleteSpriteAOP(const std::vector<Sprite*>& sprites)
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

void DeleteSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		InsertSpriteSJ::Instance()->Insert(m_sprites[i]);
	}
}

void DeleteSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(m_sprites[i]);
	}
}

Json::Value DeleteSpriteAOP::Store(const std::vector<Sprite*>& sprites) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprites, sprites);
	val["type"] = AT_DELETE;
	return val;
}

}