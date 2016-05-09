#ifndef _EASYEDITOR_TRANSLATE_SPRITE_AOP_H_
#define _EASYEDITOR_TRANSLATE_SPRITE_AOP_H_

#include "AtomicOP.h"

#include <SM_Vector.h>

namespace ee
{

class Sprite;
class SpriteSelection;

class TranslateSpriteAOP : public AtomicOP
{
public:
	TranslateSpriteAOP(const SpriteSelection& selection, const sm::vec2& offset);
	TranslateSpriteAOP(Sprite* sprite, const sm::vec2& offset);
	TranslateSpriteAOP(const std::vector<Sprite*>& sprites, const sm::vec2& offset);
	virtual ~TranslateSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	std::vector<Sprite*> m_sprites;

	sm::vec2 m_offset;

}; // TranslateSpriteAOP 

}

#endif // _EASYEDITOR_TRANSLATE_SPRITE_AOP_H_