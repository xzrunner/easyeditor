#include "SetSpriteColorAOP.h"
#include "Sprite.h"

#include <sprite2/RenderColor.h>

#include <algorithm>

#include <assert.h>

namespace ee
{

#define SET_SPRITE_COLOR_AOP_DEF(name, var) \
	\
	SetSprite##name##ColorAOP::SetSprite##name##ColorAOP(Sprite* sprite, const s2::Color& color) \
		: m_new_color(color) \
	{ \
		sprite->AddReference(); \
		m_sprites.push_back(sprite); \
		m_old_color.push_back(sprite->Color().##var##); \
	} \
	\
	SetSprite##name##ColorAOP::SetSprite##name##ColorAOP(const std::vector<Sprite*>& sprites, const s2::Color& color) \
		: m_new_color(color) \
	{ \
		for_each(sprites.begin(), sprites.end(), cu::AddRefFonctor<Sprite>()); \
		m_sprites = sprites; \
	\
		for (int i = 0, n = sprites.size(); i < n; ++i) { \
			m_old_color.push_back(m_sprites[i]->Color().##var##); \
		} \
	} \
	\
	SetSprite##name##ColorAOP::~SetSprite##name##ColorAOP() \
	{ \
		for_each(m_sprites.begin(), m_sprites.end(), cu::RemoveRefFonctor<Sprite>()); \
	} \
	\
	void SetSprite##name##ColorAOP::Undo() \
	{ \
		assert(m_sprites.size() == m_old_color.size()); \
		for (int i = 0, n = m_sprites.size(); i < n; ++i) { \
			m_sprites[i]->Color().##var## = m_old_color[i]; \
		} \
	} \
	\
	void SetSprite##name##ColorAOP::Redo() \
	{ \
		for (int i = 0, n = m_sprites.size(); i < n; ++i) { \
			m_sprites[i]->Color().##var## = m_new_color; \
		} \
	} \
	\
	Json::Value SetSprite##name##ColorAOP::Store(const std::vector<Sprite*>& sprites) const \
	{ \
		Json::Value ret; \
		return ret; \
	} \

SET_SPRITE_COLOR_AOP_DEF(Mul, mul)
SET_SPRITE_COLOR_AOP_DEF(Add, add)
SET_SPRITE_COLOR_AOP_DEF(TransR, rmap)
SET_SPRITE_COLOR_AOP_DEF(TransG, gmap)
SET_SPRITE_COLOR_AOP_DEF(TransB, bmap)

}
