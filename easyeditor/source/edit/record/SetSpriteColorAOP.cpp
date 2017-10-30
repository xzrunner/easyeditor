#include "SetSpriteColorAOP.h"
#include "Sprite.h"

#include <sprite2/RenderColor.h>

#include <algorithm>

#include <assert.h>

namespace ee
{

#define SET_SPRITE_COLOR_AOP_DEF(name, var)                                                                       \
	                                                                                                              \
	SetSprite##name##ColorAOP::SetSprite##name##ColorAOP(const SprPtr& spr, const s2::Color& color)               \
		: m_new_color(color)                                                                                      \
	{                                                                                                             \
		m_sprs.push_back(spr);                                                                                    \
		m_old_color.push_back(spr->GetColor().Get##var##());                                                      \
	}                                                                                                             \
	                                                                                                              \
	SetSprite##name##ColorAOP::SetSprite##name##ColorAOP(const std::vector<SprPtr>& sprs, const s2::Color& color) \
		: m_new_color(color)                                                                                      \
		, m_sprs(sprs)                                                                                            \
	{                                                                                                             \
		for (auto& spr : m_sprs) {                                                                                 \
			m_old_color.push_back(spr->GetColor().Get##var##());                                                  \
		}                                                                                                         \
	}                                                                                                             \
	                                                                                                              \
	void SetSprite##name##ColorAOP::Undo()                                                                        \
	{                                                                                                             \
		assert(m_sprs.size() == m_old_color.size());                                                              \
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {                                                          \
			s2::RenderColor rc = m_sprs[i]->GetColor();                                                           \
			rc.Set##var##(m_old_color[i]);                                                                        \
			m_sprs[i]->SetColor(rc);                                                                              \
		}                                                                                                         \
	}                                                                                                             \
	                                                                                                              \
	void SetSprite##name##ColorAOP::Redo()                                                                        \
	{                                                                                                             \
		for (auto& spr : m_sprs) {                                                                                 \
			s2::RenderColor rc = spr->GetColor();                                                                 \
			rc.Set##var##(m_new_color);                                                                           \
			spr->SetColor(rc);                                                                                    \
		}                                                                                                         \
	}                                                                                                             \
	                                                                                                              \
	Json::Value SetSprite##name##ColorAOP::Store(const std::vector<SprPtr>& sprs) const                          \
	{                                                                                                             \
		Json::Value ret;                                                                                          \
		return ret;                                                                                               \
	}                                                                                                             \

SET_SPRITE_COLOR_AOP_DEF(Mul, Mul)
SET_SPRITE_COLOR_AOP_DEF(Add, Add)
SET_SPRITE_COLOR_AOP_DEF(TransR, RMap)
SET_SPRITE_COLOR_AOP_DEF(TransG, GMap)
SET_SPRITE_COLOR_AOP_DEF(TransB, BMap)

}
