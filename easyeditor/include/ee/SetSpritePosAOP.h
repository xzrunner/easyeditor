#ifndef _EASYEDITOR_SET_SPRITE_POS_AOP_H_
#define _EASYEDITOR_SET_SPRITE_POS_AOP_H_

#include "AtomicOP.h"

#include <SM_Vector.h>

namespace ee
{

class Sprite;

class SetSpritePosAOP : public AtomicOP
{
public:
	SetSpritePosAOP(Sprite* sprite, const sm::vec2& pos);
	SetSpritePosAOP(const std::vector<Sprite*>& sprites, const sm::vec2& pos);
	virtual ~SetSpritePosAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	std::vector<Sprite*> m_sprites;
	std::vector<sm::vec2> m_old_pos;

	sm::vec2 m_new_pos;

}; // SetSpritePosAOP

}

#endif // _EASYEDITOR_SET_SPRITE_POS_AOP_H_