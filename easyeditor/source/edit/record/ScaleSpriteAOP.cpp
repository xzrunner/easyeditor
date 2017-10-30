#include "ScaleSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "Sprite.h"

namespace ee
{

ScaleSpriteAOP::ScaleSpriteAOP(const SprPtr& spr,
							   const sm::vec2& new_scale, 
							   const sm::vec2& old_scale)
	: m_new_scale(new_scale)
	, m_old_scale(old_scale)
{
	m_sprs.push_back(spr);
}

void ScaleSpriteAOP::Undo()
{
	for (auto& spr : m_sprs) {
		spr->SetScale(m_old_scale);
	} 
}

void ScaleSpriteAOP::Redo()
{
	for (auto& spr : m_sprs) {
		spr->SetScale(m_new_scale);
	} 
}

void ScaleSpriteAOP::Copy(const std::vector<SprPtr>& sprs)
{
	for (auto& spr : sprs) {
		spr->SetScale(m_new_scale);
	}
}

Json::Value ScaleSpriteAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprs, sprs);
	val["type"] = AT_SCALE;
	val["new_scale_x"] = m_new_scale.x;
	val["new_scale_y"] = m_new_scale.y;
	val["old_scale_x"] = m_old_scale.x;
	val["old_scale_y"] = m_old_scale.y;
	return val;
}

}