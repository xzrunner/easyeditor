#ifndef _EASYEDITOR_SHEAR_SPRITE_AOP_H_
#define _EASYEDITOR_SHEAR_SPRITE_AOP_H_

#include "AtomicOP.h"

#include <SM_Vector.h>

namespace ee
{

class ShearSpriteAOP : public AtomicOP
{
public:
	ShearSpriteAOP(Sprite* spr, const sm::vec2& new_shear, 
		const sm::vec2& old_shear);
	virtual ~ShearSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprs) const;

private:
	std::vector<Sprite*> m_sprs;

	sm::vec2 m_old_shear, m_new_shear;

}; // ShearSpriteAOP 

}

#endif // _EASYEDITOR_SHEAR_SPRITE_AOP_H_