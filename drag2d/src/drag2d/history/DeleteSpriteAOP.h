#ifndef _DRAG2D_DELETE_SPRITE_AOP_H_
#define _DRAG2D_DELETE_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

namespace d2d
{

class DeleteSpriteAOP : public AbstractAtomicOP
{
public:
	DeleteSpriteAOP(const std::vector<ISprite*>& sprites);
	virtual ~DeleteSpriteAOP();

	virtual void undo();
	virtual void redo();

	virtual Json::Value store(const std::vector<ISprite*>& sprites);

private:
	std::vector<ISprite*> m_sprites;

}; // DeleteSpriteAOP

}

#endif // _DRAG2D_DELETE_SPRITE_AOP_H_