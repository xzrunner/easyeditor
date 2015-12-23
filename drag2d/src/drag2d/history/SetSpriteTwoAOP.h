#ifndef _DRAG2D_SET_SPRITE_SCALE_AOP_H_
#define _DRAG2D_SET_SPRITE_SCALE_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class ISprite;

#define SET_SPRITE_TWO_AOP_DEC(name) \
	class SetSprite##name##AOP : public AbstractAtomicOP \
	{ \
	public: \
		SetSprite##name##AOP(const std::vector<ISprite*>& sprites, const Vector& val); \
		virtual ~SetSprite##name##AOP(); \
	\
		virtual void Undo(); \
		virtual void Redo(); \
	\
		virtual Json::Value Store(const std::vector<ISprite*>& sprites) const; \
	\
	private: \
		std::vector<ISprite*> m_sprites; \
		std::vector<Vector> m_old; \
	\
		Vector m_new; \
	}; \

SET_SPRITE_TWO_AOP_DEC(Scale)
SET_SPRITE_TWO_AOP_DEC(Offset)
SET_SPRITE_TWO_AOP_DEC(Shear)
SET_SPRITE_TWO_AOP_DEC(Perspective)

}

#endif // _DRAG2D_SET_SPRITE_SCALE_AOP_H_