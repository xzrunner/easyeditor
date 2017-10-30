#include "InsertSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "Sprite.h"
#include "MultiSpritesImpl.h"
#include "sprite_msg.h"

#include <algorithm>

namespace ee
{

InsertSpriteAOP::InsertSpriteAOP(const SprPtr& spr)
{
	m_sprs.push_back(spr);
}

InsertSpriteAOP::InsertSpriteAOP(const std::vector<SprPtr>& sprs)
	: m_sprs(sprs)
{	
}

void InsertSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(m_sprs[i]);
	}
}

void InsertSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		InsertSpriteSJ::Instance()->Insert(m_sprs[i]);
	}
}

Json::Value InsertSpriteAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprs, sprs);
	val["type"] = AT_INSERT;
	return val;
}

}