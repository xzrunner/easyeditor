#ifndef _DRAG2D_SHEAR_SPRITE_AOP_H_
#define _DRAG2D_SHEAR_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class ShearSpriteAOP : public AbstractAtomicOP
{
public:
	ShearSpriteAOP(Sprite* sprite, const Vector& new_shear, 
		const Vector& old_shear);
	virtual ~ShearSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	std::vector<Sprite*> m_sprites;

	Vector m_old_shear, m_new_shear;

}; // ShearSpriteAOP 

}

#endif // _DRAG2D_SHEAR_SPRITE_AOP_H_