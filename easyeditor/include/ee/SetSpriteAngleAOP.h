#ifndef _EASYEDITOR_SET_SPRITE_POS_AOP_H_
#define _EASYEDITOR_SET_SPRITE_POS_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

namespace ee
{

class SetSpriteAngleAOP : public AtomicOP
{
public:
	SetSpriteAngleAOP(const SprPtr& spr, float angle);
	SetSpriteAngleAOP(const std::vector<SprPtr>& sprs, float angle);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	std::vector<SprPtr> m_sprs;
	std::vector<float> m_old_angle;

	float m_new_angle;

}; // SetSpriteAngleAOP

}

#endif // _EASYEDITOR_SET_SPRITE_POS_AOP_H_