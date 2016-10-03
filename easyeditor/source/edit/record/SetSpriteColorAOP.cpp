#include "SetSpriteColorAOP.h"
#include "Sprite.h"

#include <sprite2/RenderColor.h>

#include <algorithm>

#include <assert.h>

namespace ee
{

#define SET_SPRITE_COLOR_AOP_DEF(name, var) \
	\
	SetSprite##name##ColorAOP::SetSprite##name##ColorAOP(Sprite* spr, const s2::Color& color) \
		: m_new_color(color) \
	{ \
		spr->AddReference(); \
		m_sprs.push_back(spr); \
		m_old_color.push_back(spr->Color().##var##); \
	} \
	\
	SetSprite##name##ColorAOP::SetSprite##name##ColorAOP(const std::vector<Sprite*>& sprs, const s2::Color& color) \
		: m_new_color(color) \
	{ \
		for_each(sprs.begin(), sprs.end(), cu::AddRefFunctor<Sprite>()); \
		m_sprs = sprs; \
	\
		for (int i = 0, n = sprs.size(); i < n; ++i) { \
			m_old_color.push_back(m_sprs[i]->Color().##var##); \
		} \
	} \
	\
	SetSprite##name##ColorAOP::~SetSprite##name##ColorAOP() \
	{ \
		for_each(m_sprs.begin(), m_sprs.end(), cu::RemoveRefFunctor<Sprite>()); \
	} \
	\
	void SetSprite##name##ColorAOP::Undo() \
	{ \
		assert(m_sprs.size() == m_old_color.size()); \
		for (int i = 0, n = m_sprs.size(); i < n; ++i) { \
			m_sprs[i]->Color().##var## = m_old_color[i]; \
		} \
	} \
	\
	void SetSprite##name##ColorAOP::Redo() \
	{ \
		for (int i = 0, n = m_sprs.size(); i < n; ++i) { \
			m_sprs[i]->Color().##var## = m_new_color; \
		} \
	} \
	\
	Json::Value SetSprite##name##ColorAOP::Store(const std::vector<Sprite*>& sprs) const \
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
