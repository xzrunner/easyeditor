#ifndef _DRAG2D_PERSPECTIVE_SPRITE_AOP_H_
#define _DRAG2D_PERSPECTIVE_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class ISprite;

class PerspectiveSpriteAOP : public AbstractAtomicOP
{
public:
	PerspectiveSpriteAOP(ISprite* sprite, const Vector& new_persp,
		const Vector& old_persp);

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ISprite*>& sprites) const;

private:
	ISprite* m_spr;

	Vector m_new_persp, m_old_persp;

}; // PerspectiveSpriteAOP

}

#endif // _DRAG2D_PERSPECTIVE_SPRITE_AOP_H_