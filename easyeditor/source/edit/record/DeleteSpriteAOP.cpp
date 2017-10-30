#include "DeleteSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "Sprite.h"
#include "MultiSpritesImpl.h"
#include "sprite_msg.h"

namespace ee
{

DeleteSpriteAOP::DeleteSpriteAOP(const std::vector<SprPtr>& sprs)
	: m_sprs(sprs)
{
}

void DeleteSpriteAOP::Undo()
{
	for (auto& spr : m_sprs) {
		InsertSpriteSJ::Instance()->Insert(spr);
	}
}

void DeleteSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(m_sprs[i]);
	}
}

Json::Value DeleteSpriteAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprs, sprs);
	val["type"] = AT_DELETE;
	return val;
}

}