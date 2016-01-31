#ifndef _EASYEDITOR_SCALE_SPRITE_AOP_H_
#define _EASYEDITOR_SCALE_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Vector.h"

namespace ee
{

class ScaleSpriteAOP : public AtomicOP
{
public:
	ScaleSpriteAOP(Sprite* sprite, const Vector& new_scale, 
		const Vector& old_scale);
	virtual ~ScaleSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	std::vector<Sprite*> m_sprites;

	Vector m_old_scale, m_new_scale;

}; // ScaleSpriteAOP 

}

#endif // _EASYEDITOR_SCALE_SPRITE_AOP_H_