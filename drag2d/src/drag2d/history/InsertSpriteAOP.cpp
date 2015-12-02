#include "InsertSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"

#include "dataset/ISprite.h"
#include "view/MultiSpritesImpl.h"

#include "message/InsertSpriteSJ.h"
#include "message/RemoveSpriteSJ.h"

#include <algorithm>

namespace d2d
{

InsertSpriteAOP::InsertSpriteAOP(const std::vector<ISprite*>& sprites)
{	
	for_each(sprites.begin(), sprites.end(), RetainObjectFunctor<ISprite>());
	m_sprites = sprites;
}

InsertSpriteAOP::~InsertSpriteAOP()
{
	for_each(m_sprites.begin(), m_sprites.end(), ReleaseObjectFunctor<ISprite>());
}

void InsertSpriteAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(m_sprites[i]);
	}
}

void InsertSpriteAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		InsertSpriteSJ::Instance()->Insert(m_sprites[i]);
	}
}

Json::Value InsertSpriteAOP::store(const std::vector<ISprite*>& sprites)
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprites, sprites);
	val["type"] = AT_INSERT;
	return val;
}

}