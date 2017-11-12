#ifndef _EASYEDITOR_TRANSLATE_SPRITE_AOP_H_
#define _EASYEDITOR_TRANSLATE_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

#include <SM_Vector.h>

namespace ee
{

class Sprite;
class SpriteSelection;

class TranslateSpriteAOP : public AtomicOP
{
public:
	TranslateSpriteAOP(const SpriteSelection& selection, const sm::vec2& offset);
	TranslateSpriteAOP(const SprPtr& spr, const sm::vec2& offset);
	TranslateSpriteAOP(const std::vector<SprPtr>& sprs, const sm::vec2& offset);

	virtual void Undo() override;
	virtual void Redo() override;
	virtual void Copy(const std::vector<SprPtr>& sprs) override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	std::vector<SprPtr> m_sprs;

	sm::vec2 m_offset;

}; // TranslateSpriteAOP 

}

#endif // _EASYEDITOR_TRANSLATE_SPRITE_AOP_H_