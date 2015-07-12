#ifndef _DRAG2D_TRANSLATE_SPRITE_AOP_H_
#define _DRAG2D_TRANSLATE_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class ISprite;
class SpriteSelection;

class TranslateSpriteAOP : public AbstractAtomicOP
{
public:
	TranslateSpriteAOP(const SpriteSelection& selection, const Vector& offset);
	TranslateSpriteAOP(ISprite* sprite, const Vector& offset);
	TranslateSpriteAOP(const std::vector<ISprite*>& sprites, const Vector& offset);
	virtual ~TranslateSpriteAOP();

	virtual void undo();
	virtual void redo();

	virtual Json::Value store(const std::vector<ISprite*>& sprites);

private:
	std::vector<ISprite*> m_sprites;

	Vector m_offset;

}; // TranslateSpriteAOP 

}

#endif // _DRAG2D_TRANSLATE_SPRITE_AOP_H_