#ifndef _EASYEDITOR_SET_SPRITE_POS_AOP_H_
#define _EASYEDITOR_SET_SPRITE_POS_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

#include <SM_Vector.h>

namespace ee
{

class SetSpritePosAOP : public AtomicOP
{
public:
	SetSpritePosAOP(const SprPtr& spr, const sm::vec2& pos);
	SetSpritePosAOP(const std::vector<SprPtr>& sprs, const sm::vec2& pos);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	std::vector<SprPtr> m_sprs;
	std::vector<sm::vec2> m_old_pos;

	sm::vec2 m_new_pos;

}; // SetSpritePosAOP

}

#endif // _EASYEDITOR_SET_SPRITE_POS_AOP_H_