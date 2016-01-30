#ifndef _DRAG2D_DELETE_SPRITE_AOP_H_
#define _DRAG2D_DELETE_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

namespace d2d
{

class DeleteSpriteAOP : public AbstractAtomicOP
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

#endif // _DRAG2D_DELETE_SPRITE_AOP_H_