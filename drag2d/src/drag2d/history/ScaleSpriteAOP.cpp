#include "ScaleSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"

#include "dataset/ISprite.h"

namespace d2d
{

ScaleSpriteAOP::ScaleSpriteAOP(ISprite* sprite, 
							   const Vector& new_scale, 
							   const Vector& old_scale)
	: m_new_scale(new_scale)
	, m_old_scale(old_scale)
{
	sprite->Retain();
	m_sprites.push_back(sprite);
}

ScaleSpriteAOP::~ScaleSpriteAOP()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
}

void ScaleSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->SetScale(m_old_scale);
	} 
}

void ScaleSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->SetScale(m_new_scale);
	} 
}

Json::Value ScaleSpriteAOP::Store(const std::vector<ISprite*>& sprites) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprites, sprites);
	val["type"] = AT_SCALE;
	val["new_scale_x"] = m_new_scale.x;
	val["new_scale_y"] = m_new_scale.y;
	val["old_scale_x"] = m_old_scale.x;
	val["old_scale_y"] = m_old_scale.y;
	return val;
}

}