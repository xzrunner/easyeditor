#ifndef _DRAG2D_SET_SPRITE_POS_AOP_H_
#define _DRAG2D_SET_SPRITE_POS_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class ISprite;

class SetSpriteAngleAOP : public AbstractAtomicOP
{
public:
	SetSpriteAngleAOP(ISprite* sprite, float angle);
	SetSpriteAngleAOP(const std::vector<ISprite*>& sprites, float angle);
	virtual ~SetSpriteAngleAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ISprite*>& sprites) const;

private:
	std::vector<ISprite*> m_sprites;
	std::vector<float> m_old_angle;

	float m_new_angle;

}; // SetSpriteAngleAOP

}

#endif // _DRAG2D_SET_SPRITE_POS_AOP_H_