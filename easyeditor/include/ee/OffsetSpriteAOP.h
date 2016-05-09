#ifndef _EASYEDITOR_OFFSET_SPRITE_AOP_H_
#define _EASYEDITOR_OFFSET_SPRITE_AOP_H_

#include "AtomicOP.h"

#include <SM_Vector.h>

namespace ee
{

class Sprite;

class OffsetSpriteAOP : public AtomicOP
{
public:
	OffsetSpriteAOP(Sprite* sprite, const sm::vec2& new_offset, 
		const sm::vec2& old_offset);

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	Sprite* m_sprite;

	sm::vec2 m_new_offset, m_old_offset;

}; // OffsetSpriteAOP

}

#endif // _EASYEDITOR_OFFSET_SPRITE_AOP_H_