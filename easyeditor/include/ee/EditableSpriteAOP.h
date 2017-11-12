#ifndef _EASYEDITOR_EDITABLE_SPRITE_AOP_H_
#define _EASYEDITOR_EDITABLE_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

namespace ee
{

class EditableSpriteAOP : public AtomicOP
{
public:
	EditableSpriteAOP(const std::vector<SprPtr>& sprs);

	virtual void Undo() override;
	virtual void Redo() override;
	virtual void Copy(const std::vector<SprPtr>& sprs) override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	std::vector<SprPtr> m_sprs;

}; // EditableSpriteAOP

}

#endif // _EASYEDITOR_EDITABLE_SPRITE_AOP_H_