#ifndef _DRAG2D_SET_SPRITE_COLOR_AOP_H_
#define _DRAG2D_SET_SPRITE_COLOR_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Color.h"

namespace d2d
{

class ISprite;

#define SET_SPRITE_COLOR_AOP_DEC(name) \
	class SetSprite##name##ColorAOP : public AbstractAtomicOP \
	{ \
	public: \
		SetSprite##name##ColorAOP(ISprite* sprite, const Colorf& color); \
		SetSprite##name##ColorAOP(const std::vector<ISprite*>& sprites, const Colorf& color); \
		virtual ~SetSprite##name##ColorAOP(); \
	\
		virtual void Undo(); \
		virtual void Redo(); \
	\
		virtual Json::Value Store(const std::vector<ISprite*>& sprites) const; \
	\
	private: \
		std::vector<ISprite*> m_sprites; \
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

#endif // _DRAG2D_SET_SPRITE_COLOR_AOP_H_