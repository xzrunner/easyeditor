#include "SetSpriteTwoAOP.h"
#include "Sprite.h"

#include <algorithm>

#include <assert.h>

namespace ee
{

#define SET_SPRITE_TWO_AOP_DEF(name, var) \
	\
	SetSprite##name##AOP::SetSprite##name##AOP(const std::vector<Sprite*>& sprites, const Vector& val)	\
		: m_new(val) \
	{ \
		for_each(sprites.begin(), sprites.end(), RetainObjectFunctor<Sprite>()); \
		m_sprites = sprites; \
	\
		for (int i = 0, n = sprites.size(); i < n; ++i) { \
			m_old.push_back(sprites[i]->Get##var##()); \
		} \
	} \
	\
	SetSprite##name##AOP::~SetSprite##name##AOP() \
	{ \
		for_each(m_sprites.begin(), m_sprites.end(), ReleaseObjectFunctor<Sprite>()); \
	} \
	\
	void SetSprite##name##AOP::Undo() \
	{ \
		assert(m_sprites.size() == m_old.size()); \
		for (int i = 0, n = m_sprites.size(); i < n; ++i) { \
			Sprite* spr = m_sprites[i]; \
			spr->Set##var##(m_old[i]); \
		} \
	} \
	\
	void SetSprite##name##AOP::Redo() \
	{ \
		for (int i = 0, n = m_sprites.size(); i < n; ++i) { \
			Sprite* spr = m_sprites[i]; \
			spr->Set##var##(m_new); \
		} \
	} \
	\
	Json::Value SetSprite##name##AOP::Store(const std::vector<Sprite*>& sprites) const \
	{ \
		Json::Value ret; \
		return ret; \
	} \

SET_SPRITE_TWO_AOP_DEF(Scale, Scale)
SET_SPRITE_TWO_AOP_DEF(Offset, Offset)
SET_SPRITE_TWO_AOP_DEF(Shear, Shear)
SET_SPRITE_TWO_AOP_DEF(Perspective, Perspective)

}