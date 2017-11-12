#ifndef _EASYEDITOR_OFFSET_SPRITE_AOP_H_
#define _EASYEDITOR_OFFSET_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

#include <SM_Vector.h>

namespace ee
{

class Sprite;

class OffsetSpriteAOP : public AtomicOP
{
public:
	OffsetSpriteAOP(const SprPtr& spr, const sm::vec2& new_offset,
		const sm::vec2& old_offset);

	virtual void Undo() override;
	virtual void Redo() override;
	virtual void Copy(const std::vector<SprPtr>& sprs) override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	SprPtr m_spr;

	sm::vec2 m_new_offset, m_old_offset;

}; // OffsetSpriteAOP

}

#endif // _EASYEDITOR_OFFSET_SPRITE_AOP_H_