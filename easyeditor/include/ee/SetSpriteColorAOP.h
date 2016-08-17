#ifndef _EASYEDITOR_SET_SPRITE_COLOR_AOP_H_
#define _EASYEDITOR_SET_SPRITE_COLOR_AOP_H_

#include "AtomicOP.h"

#include <sprite2/Color.h>

namespace ee
{

class Sprite;

#define SET_SPRITE_COLOR_AOP_DEC(name) \
	class SetSprite##name##ColorAOP : public AtomicOP \
	{ \
	public: \
		SetSprite##name##ColorAOP(Sprite* spr, const s2::Color& color); \
		SetSprite##name##ColorAOP(const std::vector<Sprite*>& sprs, const s2::Color& color); \
		virtual ~SetSprite##name##ColorAOP(); \
	\
		virtual void Undo(); \
		virtual void Redo(); \
	\
		virtual Json::Value Store(const std::vector<Sprite*>& sprs) const; \
	\
	private: \
		std::vector<Sprite*> m_sprs; \
		std::vector<s2::Color> m_old_color; \
	\
		s2::Color m_new_color; \
	}; \

SET_SPRITE_COLOR_AOP_DEC(Mul)
SET_SPRITE_COLOR_AOP_DEC(Add)
SET_SPRITE_COLOR_AOP_DEC(TransR)
SET_SPRITE_COLOR_AOP_DEC(TransG)
SET_SPRITE_COLOR_AOP_DEC(TransB)

}

#endif // _EASYEDITOR_SET_SPRITE_COLOR_AOP_H_