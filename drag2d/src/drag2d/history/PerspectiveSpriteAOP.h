#ifndef _DRAG2D_PERSPECTIVE_SPRITE_AOP_H_
#define _DRAG2D_PERSPECTIVE_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class PerspectiveSpriteAOP : public AbstractAtomicOP
{
public:
	PerspectiveSpriteAOP(ISprite* sprite, const Vector& new_persp,
		const Vector& old_persp);

	virtual void undo();
	virtual void redo();

	virtual Json::Value store(const std::vector<ISprite*>& sprites);

private:
	ISprite* m_spr;

	Vector m_new_persp, m_old_persp;

}; // PerspectiveSpriteAOP

}

#endif // _DRAG2D_PERSPECTIVE_SPRITE_AOP_H_