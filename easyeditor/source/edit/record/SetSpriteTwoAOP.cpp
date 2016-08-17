#include "SetSpriteTwoAOP.h"
#include "Sprite.h"

#include <algorithm>

#include <assert.h>

namespace ee
{

#define SET_SPRITE_TWO_AOP_DEF(name, var) \
	\
	SetSprite##name##AOP::SetSprite##name##AOP(const std::vector<Sprite*>& sprs, const sm::vec2& val)	\
		: m_new(val) \
	{ \
		for_each(sprs.begin(), sprs.end(), cu::AddRefFonctor<Sprite>()); \
		m_sprs = sprs; \
	\
		for (int i = 0, n = sprs.size(); i < n; ++i) { \
			m_old.push_back(sprs[i]->Get##var##()); \
		} \
	} \
	\
	SetSprite##name##AOP::~SetSprite##name##AOP() \
	{ \
		for_each(m_sprs.begin(), m_sprs.end(), cu::RemoveRefFonctor<Sprite>()); \
	} \
	\
	void SetSprite##name##AOP::Undo() \
	{ \
		assert(m_sprs.size() == m_old.size()); \
		for (int i = 0, n = m_sprs.size(); i < n; ++i) { \
			Sprite* spr = m_sprs[i]; \
			spr->Set##var##(m_old[i]); \
		} \
	} \
	\
	void SetSprite##name##AOP::Redo() \
	{ \
		for (int i = 0, n = m_sprs.size(); i < n; ++i) { \
			Sprite* spr = m_sprs[i]; \
			spr->Set##var##(m_new); \
		} \
	} \
	\
	Json::Value SetSprite##name##AOP::Store(const std::vector<Sprite*>& sprs) const \
	{ \
		Json::Value ret; \
		return ret; \
	} \

SET_SPRITE_TWO_AOP_DEF(Scale, Scale)
SET_SPRITE_TWO_AOP_DEF(Offset, Offset)
SET_SPRITE_TWO_AOP_DEF(Shear, Shear)
SET_SPRITE_TWO_AOP_DEF(Perspective, Perspective)

}