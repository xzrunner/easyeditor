#ifndef _EASYEDITOR_PERSPECTIVE_SPRITE_AOP_H_
#define _EASYEDITOR_PERSPECTIVE_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Vector.h"

namespace ee
{

class Sprite;

class PerspectiveSpriteAOP : public AtomicOP
{
public:
	PerspectiveSpriteAOP(Sprite* sprite, const Vector& new_persp,
		const Vector& old_persp);

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	Sprite* m_spr;

	Vector m_new_persp, m_old_persp;

}; // PerspectiveSpriteAOP

}

#endif // _EASYEDITOR_PERSPECTIVE_SPRITE_AOP_H_