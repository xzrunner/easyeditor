#ifndef _EASYEDITOR_PERSPECTIVE_SPRITE_AOP_H_
#define _EASYEDITOR_PERSPECTIVE_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

#include <SM_Vector.h>

namespace ee
{

class PerspectiveSpriteAOP : public AtomicOP
{
public:
	PerspectiveSpriteAOP(const SprPtr& spr, const sm::vec2& new_persp,
		const sm::vec2& old_persp);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	SprPtr m_spr;

	sm::vec2 m_new_persp, m_old_persp;

}; // PerspectiveSpriteAOP

}

#endif // _EASYEDITOR_PERSPECTIVE_SPRITE_AOP_H_