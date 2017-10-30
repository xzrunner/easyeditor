#ifndef _EASYEDITOR_SCALE_SPRITE_AOP_H_
#define _EASYEDITOR_SCALE_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

#include <SM_Vector.h>

namespace ee
{

class ScaleSpriteAOP : public AtomicOP
{
public:
	ScaleSpriteAOP(const SprPtr& spr, const sm::vec2& new_scale, 
		const sm::vec2& old_scale);

	virtual void Undo();
	virtual void Redo();
	virtual void Copy(const std::vector<SprPtr>& sprs);

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const;

private:
	std::vector<SprPtr> m_sprs;

	sm::vec2 m_old_scale, m_new_scale;

}; // ScaleSpriteAOP 

}

#endif // _EASYEDITOR_SCALE_SPRITE_AOP_H_