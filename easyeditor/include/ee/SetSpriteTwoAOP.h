#ifndef _EASYEDITOR_SET_SPRITE_SCALE_AOP_H_
#define _EASYEDITOR_SET_SPRITE_SCALE_AOP_H_

#include "AtomicOP.h"

#include <SM_Vector.h>

namespace ee
{

class Sprite;

#define SET_SPRITE_TWO_AOP_DEC(name) \
	class SetSprite##name##AOP : public AtomicOP \
	{ \
	public: \
		SetSprite##name##AOP(const std::vector<Sprite*>& sprites, const sm::vec2& val); \
		virtual ~SetSprite##name##AOP(); \
	\
		virtual void Undo(); \
		virtual void Redo(); \
	\
		virtual Json::Value Store(const std::vector<Sprite*>& sprites) const; \
	\
	private: \
		std::vector<Sprite*> m_sprites; \
		std::vector<sm::vec2> m_old; \
	\
		sm::vec2 m_new; \
	}; \

SET_SPRITE_TWO_AOP_DEC(Scale)
SET_SPRITE_TWO_AOP_DEC(Offset)
SET_SPRITE_TWO_AOP_DEC(Shear)
SET_SPRITE_TWO_AOP_DEC(Perspective)

}

#endif // _EASYEDITOR_SET_SPRITE_SCALE_AOP_H_