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
	sprite->retain();
	m_sprites.push_back(sprite);
}

ScaleSpriteAOP::~ScaleSpriteAOP()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->release();
	}
}

void ScaleSpriteAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* sprite = m_sprites[i];
		sprite->setScale(m_old_scale.x, m_old_scale.y);
	} 
}

void ScaleSpriteAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* sprite = m_sprites[i];
		sprite->setScale(m_new_scale.x, m_new_scale.y);
	} 
}

Json::Value ScaleSpriteAOP::store(const std::vector<ISprite*>& sprites)
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