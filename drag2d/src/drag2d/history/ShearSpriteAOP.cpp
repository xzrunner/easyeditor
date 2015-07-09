#include "ShearSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"

#include "dataset/ISprite.h"

namespace d2d
{

ShearSpriteAOP::ShearSpriteAOP(ISprite* sprite, 
							   const Vector& new_shear, 
							   const Vector& old_shear)
   : m_new_shear(new_shear)
   , m_old_shear(old_shear)
{
	sprite->Retain();
	m_sprites.push_back(sprite);
}

ShearSpriteAOP::~ShearSpriteAOP()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
}

void ShearSpriteAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) 
	{
		ISprite* sprite = m_sprites[i];
		sprite->SetShear(m_old_shear.x, m_old_shear.y);
	} 
}

void ShearSpriteAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) 
	{
		ISprite* sprite = m_sprites[i];
		sprite->SetShear(m_new_shear.x, m_new_shear.y);
	} 
}

Json::Value ShearSpriteAOP::store(const std::vector<ISprite*>& sprites)
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprites, sprites);
	val["type"] = AT_SHEAR;
	val["new_shear_x"] = m_new_shear.x;
	val["new_shear_y"] = m_new_shear.y;
	val["old_shear_x"] = m_old_shear.x;
	val["old_shear_y"] = m_old_shear.y;
	return val;
}

}