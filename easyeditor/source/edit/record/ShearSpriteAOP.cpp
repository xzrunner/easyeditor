#include "ShearSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "Sprite.h"

namespace ee
{

ShearSpriteAOP::ShearSpriteAOP(Sprite* spr, 
							   const sm::vec2& new_shear, 
							   const sm::vec2& old_shear)
   : m_new_shear(new_shear)
   , m_old_shear(old_shear)
{
	spr->AddReference();
	m_sprs.push_back(spr);
}

ShearSpriteAOP::~ShearSpriteAOP()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->RemoveReference();
	}
}

void ShearSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetShear(m_old_shear);
	}
}

void ShearSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetShear(m_new_shear);
	} 
}

void ShearSpriteAOP::Copy(const std::vector<ee::Sprite*>& sprs)
{
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->SetShear(m_new_shear);
	}	
}

Json::Value ShearSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
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