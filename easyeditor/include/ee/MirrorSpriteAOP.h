#ifndef _EASYEDITOR_MIRROR_SPRITE_AOP_H_
#define _EASYEDITOR_MIRROR_SPRITE_AOP_H_

#include "AtomicOP.h"

#include <SM_Vector.h>

namespace ee
{

class MirrorSpriteAOP : public AtomicOP
{
public:
	MirrorSpriteAOP(Sprite* spr, bool old_mirror_x, bool old_mirror_y,
		bool new_mirror_x, bool new_mirror_y);
	virtual ~MirrorSpriteAOP();

	virtual void Undo();
	virtual void Redo();
	virtual void Copy(const std::vector<ee::Sprite*>& sprs);

	virtual Json::Value Store(const std::vector<Sprite*>& sprs) const;

private:
	static void SetMirror(Sprite* spr, const sm::bvec2& mirror);

private:
	Sprite* m_spr;

	sm::bvec2 m_old_mirror, m_new_mirror;

}; // MirrorSpriteAOP 

}

#endif // _EASYEDITOR_MIRROR_SPRITE_AOP_H_