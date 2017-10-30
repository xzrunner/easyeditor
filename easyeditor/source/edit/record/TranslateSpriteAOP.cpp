#include "TranslateSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "SpriteSelection.h"
#include "FetchAllVisitor.h"

#include <algorithm>

namespace ee
{

TranslateSpriteAOP::TranslateSpriteAOP(const SpriteSelection& selection, const sm::vec2& offset)
	: m_offset(offset)
{
	selection.Traverse(FetchAllRefVisitor<Sprite>(m_sprs));
}

TranslateSpriteAOP::TranslateSpriteAOP(const SprPtr& spr, const sm::vec2& offset)
	: m_offset(offset)
{
	m_sprs.push_back(spr);
}

TranslateSpriteAOP::TranslateSpriteAOP(const std::vector<SprPtr>& sprs, const sm::vec2& offset)
	: m_offset(offset)
	, m_sprs(sprs)
{
}

void TranslateSpriteAOP::Undo()
{
	for (auto& spr : m_sprs) {
		spr->Translate(-m_offset);
	}
}

void TranslateSpriteAOP::Redo()
{
	for (auto& spr : m_sprs) {
		spr->Translate(m_offset);
	}
}

void TranslateSpriteAOP::Copy(const std::vector<SprPtr>& sprs)
{
	for (auto& spr : sprs) {
		spr->Translate(m_offset);
	}
}

Json::Value TranslateSpriteAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprs, sprs);
	val["type"] = AT_MOVE;
	val["dx"] = m_offset.x;
	val["dy"] = m_offset.y;
	return val;
}

}