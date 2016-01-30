#ifndef _DRAG2D_OFFSET_SPRITE_AOP_H_
#define _DRAG2D_OFFSET_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class Sprite;

class OffsetSpriteAOP : public AbstractAtomicOP
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

#endif // _DRAG2D_OFFSET_SPRITE_AOP_H_