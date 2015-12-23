#ifndef _DRAG2D_SET_SPRITE_POS_AOP_H_
#define _DRAG2D_SET_SPRITE_POS_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class ISprite;

class SetSpritePosAOP : public AbstractAtomicOP
{
public:
	SetSpritePosAOP(ISprite* sprite, const Vector& pos);
	SetSpritePosAOP(const std::vector<ISprite*>& sprites, const Vector& pos);
	virtual ~SetSpritePosAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ISprite*>& sprites) const;

private:
	std::vector<ISprite*> m_sprites;
	std::vector<Vector> m_old_pos;

	Vector m_new_pos;

}; // SetSpritePosAOP

}

#endif // _DRAG2D_SET_SPRITE_POS_AOP_H_