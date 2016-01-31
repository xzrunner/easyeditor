#ifndef _EASYEDITOR_SET_SPRITE_COLOR_AOP_H_
#define _EASYEDITOR_SET_SPRITE_COLOR_AOP_H_

#include "AtomicOP.h"
#include "Color.h"

namespace ee
{

class Sprite;

#define SET_SPRITE_COLOR_AOP_DEC(name) \
	class SetSprite##name##ColorAOP : public AtomicOP \
	{ \
	public: \
		SetSprite##name##ColorAOP(Sprite* sprite, const Colorf& color); \
		SetSprite##name##ColorAOP(const std::vector<Sprite*>& sprites, const Colorf& color); \
		virtual ~SetSprite##name##ColorAOP(); \
	\
		virtual void Undo(); \
		virtual void Redo(); \
	\
		virtual Json::Value Store(const std::vector<Sprite*>& sprites) const; \
	\
	private: \
		std::vector<Sprite*> m_sprites; \
		std::vector<Colorf> m_old_color; \
	\
		Colorf m_new_color; \
	}; \

SET_SPRITE_COLOR_AOP_DEC(Mul)
SET_SPRITE_COLOR_AOP_DEC(Add)
SET_SPRITE_COLOR_AOP_DEC(TransR)
SET_SPRITE_COLOR_AOP_DEC(TransG)
SET_SPRITE_COLOR_AOP_DEC(TransB)

}

#endif // _EASYEDITOR_SET_SPRITE_COLOR_AOP_H_