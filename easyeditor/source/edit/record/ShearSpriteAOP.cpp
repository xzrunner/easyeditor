#include "ShearSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "Sprite.h"

namespace ee
{

ShearSpriteAOP::ShearSpriteAOP(const SprPtr& spr,
							   const sm::vec2& new_shear, 
							   const sm::vec2& old_shear)
   : m_new_shear(new_shear)
   , m_old_shear(old_shear)
{
	m_sprs.push_back(spr);
}

void ShearSpriteAOP::Undo()
{
	for (auto& spr : m_sprs) {
		spr->SetShear(m_old_shear);
	}
}

void ShearSpriteAOP::Redo()
{
	for (auto& spr : m_sprs) {
		spr->SetShear(m_new_shear);
	} 
}

void ShearSpriteAOP::Copy(const std::vector<SprPtr>& sprs)
{
	for (auto& spr : sprs) {
		spr->SetShear(m_new_shear);
	}	
}

Json::Value ShearSpriteAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprs, sprs);
	val["type"] = AT_SHEAR;
	val["new_shear_x"] = m_new_shear.x;
	val["new_shear_y"] = m_new_shear.y;
	val["old_shear_x"] = m_old_shear.x;
	val["old_shear_y"] = m_old_shear.y;
	return val;
}

}