#ifndef _EASYEDITOR_SHEAR_SPRITE_AOP_H_
#define _EASYEDITOR_SHEAR_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

#include <SM_Vector.h>

namespace ee
{

class ShearSpriteAOP : public AtomicOP
{
public:
	ShearSpriteAOP(const SprPtr& spr, const sm::vec2& new_shear,
		const sm::vec2& old_shear);

	virtual void Undo();
	virtual void Redo();
	virtual void Copy(const std::vector<SprPtr>& sprs);

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const;

private:
	std::vector<SprPtr> m_sprs;

	sm::vec2 m_old_shear, m_new_shear;

}; // ShearSpriteAOP 

}

#endif // _EASYEDITOR_SHEAR_SPRITE_AOP_H_