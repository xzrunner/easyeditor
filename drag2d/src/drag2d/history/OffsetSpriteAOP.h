#ifndef _DRAG2D_OFFSET_SPRITE_AOP_H_
#define _DRAG2D_OFFSET_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class ISprite;

class OffsetSpriteAOP : public AbstractAtomicOP
{
public:
	OffsetSpriteAOP(ISprite* sprite, const Vector& new_offset, 
		const Vector& old_offset);

	virtual void undo();
	virtual void redo();

	virtual Json::Value store(const std::vector<ISprite*>& sprites);

private:
	ISprite* m_sprite;

	Vector m_new_offset, m_old_offset;

}; // OffsetSpriteAOP

}

#endif // _DRAG2D_OFFSET_SPRITE_AOP_H_