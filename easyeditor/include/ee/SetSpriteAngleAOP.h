#ifndef _EASYEDITOR_SET_SPRITE_POS_AOP_H_
#define _EASYEDITOR_SET_SPRITE_POS_AOP_H_

#include "AtomicOP.h"
#include "Vector.h"

namespace ee
{

class Sprite;

class SetSpriteAngleAOP : public AtomicOP
{
public:
	SetSpriteAngleAOP(Sprite* sprite, float angle);
	SetSpriteAngleAOP(const std::vector<Sprite*>& sprites, float angle);
	virtual ~SetSpriteAngleAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	std::vector<Sprite*> m_sprites;
	std::vector<float> m_old_angle;

	float m_new_angle;

}; // SetSpriteAngleAOP

}

#endif // _EASYEDITOR_SET_SPRITE_POS_AOP_H_