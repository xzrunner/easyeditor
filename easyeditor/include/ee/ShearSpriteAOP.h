#ifndef _EASYEDITOR_SHEAR_SPRITE_AOP_H_
#define _EASYEDITOR_SHEAR_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Vector.h"

namespace ee
{

class ShearSpriteAOP : public AtomicOP
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

#endif // _EASYEDITOR_SHEAR_SPRITE_AOP_H_