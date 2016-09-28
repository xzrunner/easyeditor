#ifndef _EASYEDITOR_SCALE_SPRITE_AOP_H_
#define _EASYEDITOR_SCALE_SPRITE_AOP_H_

#include "AtomicOP.h"

#include <SM_Vector.h>

namespace ee
{

class ScaleSpriteAOP : public AtomicOP
{
public:
	ScaleSpriteAOP(Sprite* spr, const sm::vec2& new_scale, 
		const sm::vec2& old_scale);
	virtual ~ScaleSpriteAOP();

	virtual void Undo();
	virtual void Redo();
	virtual void Copy(const std::vector<ee::Sprite*>& sprs);

	virtual Json::Value Store(const std::vector<Sprite*>& sprs) const;

private:
	std::vector<Sprite*> m_sprs;

	sm::vec2 m_old_scale, m_new_scale;

}; // ScaleSpriteAOP 

}

#endif // _EASYEDITOR_SCALE_SPRITE_AOP_H_