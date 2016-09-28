#include "ScaleSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "Sprite.h"

namespace ee
{

ScaleSpriteAOP::ScaleSpriteAOP(Sprite* spr, 
							   const sm::vec2& new_scale, 
							   const sm::vec2& old_scale)
	: m_new_scale(new_scale)
	, m_old_scale(old_scale)
{
	spr->AddReference();
	m_sprs.push_back(spr);
}

ScaleSpriteAOP::~ScaleSpriteAOP()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->RemoveReference();
	}
}

void ScaleSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetScale(m_old_scale);
	} 
}

void ScaleSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetScale(m_new_scale);
	} 
}

void ScaleSpriteAOP::Copy(const std::vector<ee::Sprite*>& sprs)
{
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->SetScale(m_new_scale);
	}
}

Json::Value ScaleSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
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