#ifndef _DRAG2D_INSERT_SPRITE_AOP_H_
#define _DRAG2D_INSERT_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

namespace d2d
{

class InsertSpriteAOP : public AbstractAtomicOP
{
public:
	InsertSpriteAOP(const std::vector<ISprite*>& sprites);
	virtual ~InsertSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ISprite*>& sprites) const;

private:
	std::vector<ISprite*> m_sprites;

}; // InsertSpriteAOP

}

#endif // _DRAG2D_INSERT_SPRITE_AOP_H_