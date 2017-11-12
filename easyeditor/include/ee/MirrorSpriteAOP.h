#ifndef _EASYEDITOR_MIRROR_SPRITE_AOP_H_
#define _EASYEDITOR_MIRROR_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

#include <SM_Vector.h>

namespace ee
{

class MirrorSpriteAOP : public AtomicOP
{
public:
	MirrorSpriteAOP(const SprPtr& spr, bool old_mirror_x, bool old_mirror_y,
		bool new_mirror_x, bool new_mirror_y);

	virtual void Undo() override;
	virtual void Redo() override;
	virtual void Copy(const std::vector<SprPtr>& sprs) override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	static void SetMirror(const SprPtr& spr, const sm::bvec2& mirror);

private:
	SprPtr m_spr;

	sm::bvec2 m_old_mirror, m_new_mirror;

}; // MirrorSpriteAOP 

}

#endif // _EASYEDITOR_MIRROR_SPRITE_AOP_H_