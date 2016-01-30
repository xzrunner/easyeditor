#ifndef _DRAG2D_PERSPECTIVE_SPRITE_AOP_H_
#define _DRAG2D_PERSPECTIVE_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class Sprite;

class PerspectiveSpriteAOP : public AbstractAtomicOP
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

#endif // _DRAG2D_PERSPECTIVE_SPRITE_AOP_H_