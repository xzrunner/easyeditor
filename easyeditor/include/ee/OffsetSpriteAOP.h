#ifndef _EASYEDITOR_OFFSET_SPRITE_AOP_H_
#define _EASYEDITOR_OFFSET_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Vector.h"

namespace ee
{

class Sprite;

class OffsetSpriteAOP : public AtomicOP
{
public:
	OffsetSpriteAOP(Sprite* sprite, const Vector& new_offset, 
		const Vector& old_offset);

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	Sprite* m_sprite;

	Vector m_new_offset, m_old_offset;

}; // OffsetSpriteAOP

}

#endif // _EASYEDITOR_OFFSET_SPRITE_AOP_H_