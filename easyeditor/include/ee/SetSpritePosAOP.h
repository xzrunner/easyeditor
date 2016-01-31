#ifndef _EASYEDITOR_SET_SPRITE_POS_AOP_H_
#define _EASYEDITOR_SET_SPRITE_POS_AOP_H_

#include "AtomicOP.h"
#include "Vector.h"

namespace ee
{

class Sprite;

class SetSpritePosAOP : public AtomicOP
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

#endif // _EASYEDITOR_SET_SPRITE_POS_AOP_H_