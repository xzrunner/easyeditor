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
	spr->AddReference();
	m_sprs.push_back(spr);
}

InsertSpriteAOP::InsertSpriteAOP(const std::vector<Sprite*>& sprs)
{	
	for_each(sprs.begin(), sprs.end(), cu::AddRefFunctor<Sprite>());
	m_sprs = sprs;
}

InsertSpriteAOP::~InsertSpriteAOP()
{
	for_each(m_sprs.begin(), m_sprs.end(), cu::RemoveRefFunctor<Sprite>());
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

Json::Value InsertSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprs, sprs);
	val["type"] = AT_INSERT;
	return val;
}

}