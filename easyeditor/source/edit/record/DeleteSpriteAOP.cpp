#include "DeleteSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "Sprite.h"
#include "MultiSpritesImpl.h"
#include "sprite_msg.h"

namespace ee
{

DeleteSpriteAOP::DeleteSpriteAOP(const std::vector<Sprite*>& sprs)
{
	for (size_t i = 0, n = sprs.size(); i < n; ++i) 
	{
		sprs[i]->AddReference();
		m_sprs.push_back(sprs[i]);
	}
}

DeleteSpriteAOP::~DeleteSpriteAOP()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->RemoveReference();
	}
}

void DeleteSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		InsertSpriteSJ::Instance()->Insert(m_sprs[i]);
	}
}

void DeleteSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(m_sprs[i]);
	}
}

Json::Value DeleteSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprs, sprs);
	val["type"] = AT_DELETE;
	return val;
}

}