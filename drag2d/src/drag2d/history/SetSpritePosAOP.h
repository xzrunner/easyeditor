#ifndef _DRAG2D_SET_SPRITE_POS_AOP_H_
#define _DRAG2D_SET_SPRITE_POS_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class Sprite;

class SetSpritePosAOP : public AbstractAtomicOP
{
public:
	SetSpritePosAOP(Sprite* sprite, const Vector& pos);
	SetSpritePosAOP(const std::vector<Sprite*>& sprites, const Vector& pos);
	virtual ~SetSpritePosAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	std::vector<Sprite*> m_sprites;
	std::vector<Vector> m_old_pos;

	Vector m_new_pos;

}; // SetSpritePosAOP

}

#endif // _DRAG2D_SET_SPRITE_POS_AOP_H_