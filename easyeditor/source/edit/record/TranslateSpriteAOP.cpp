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
	selection.Traverse(FetchAllVisitor<Sprite>(m_sprs));
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->AddReference();
	}
}

TranslateSpriteAOP::TranslateSpriteAOP(Sprite* spr, const sm::vec2& offset)
	: m_offset(offset)
{
	spr->AddReference();
	m_sprs.push_back(spr);
}

TranslateSpriteAOP::TranslateSpriteAOP(const std::vector<Sprite*>& sprs, const sm::vec2& offset)
	: m_offset(offset)
{
	for_each(sprs.begin(), sprs.end(), cu::AddRefFonctor<Sprite>());
	m_sprs = sprs;
}

TranslateSpriteAOP::~TranslateSpriteAOP()
{
	for_each(m_sprs.begin(), m_sprs.end(), cu::RemoveRefFonctor<Sprite>());
}

void TranslateSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->Translate(-m_offset);
	}
}

void TranslateSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->Translate(m_offset);
	}
}

void TranslateSpriteAOP::Copy(const std::vector<ee::Sprite*>& sprs)
{
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->Translate(m_offset);
	}
}

Json::Value TranslateSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprs, sprs);
	val["type"] = AT_MOVE;
	val["dx"] = m_offset.x;
	val["dy"] = m_offset.y;
	return val;
}

}