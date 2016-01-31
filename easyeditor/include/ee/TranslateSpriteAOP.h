#ifndef _EASYEDITOR_TRANSLATE_SPRITE_AOP_H_
#define _EASYEDITOR_TRANSLATE_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Vector.h"

namespace ee
{

class Sprite;
class SpriteSelection;

class TranslateSpriteAOP : public AtomicOP
{
public:
	TranslateSpriteAOP(const SpriteSelection& selection, const Vector& offset);
	TranslateSpriteAOP(Sprite* sprite, const Vector& offset);
	TranslateSpriteAOP(const std::vector<Sprite*>& sprites, const Vector& offset);
	virtual ~TranslateSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	std::vector<Sprite*> m_sprites;

	Vector m_offset;

}; // TranslateSpriteAOP 

}

#endif // _EASYEDITOR_TRANSLATE_SPRITE_AOP_H_