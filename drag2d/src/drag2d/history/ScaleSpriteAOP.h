#ifndef _DRAG2D_SCALE_SPRITE_AOP_H_
#define _DRAG2D_SCALE_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class ScaleSpriteAOP : public AbstractAtomicOP
{
public:
	ScaleSpriteAOP(ISprite* sprite, const Vector& new_scale, 
		const Vector& old_scale);
	virtual ~ScaleSpriteAOP();

	virtual void undo();
	virtual void redo();

	virtual Json::Value store(const std::vector<ISprite*>& sprites);

private:
	std::vector<ISprite*> m_sprites;

	Vector m_old_scale, m_new_scale;

}; // ScaleSpriteAOP 

}

#endif // _DRAG2D_SCALE_SPRITE_AOP_H_