#ifndef _EASYEDITOR_DELETE_SPRITE_AOP_H_
#define _EASYEDITOR_DELETE_SPRITE_AOP_H_

#include "AtomicOP.h"

namespace ee
{

class DeleteSpriteAOP : public AtomicOP
{
public:
	DeleteSpriteAOP(const std::vector<Sprite*>& sprites);
	virtual ~DeleteSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	std::vector<Sprite*> m_sprites;

}; // DeleteSpriteAOP

}

#endif // _EASYEDITOR_DELETE_SPRITE_AOP_H_