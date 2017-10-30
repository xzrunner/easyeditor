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

	virtual void Undo();
	virtual void Redo();
	virtual void Copy(const std::vector<SprPtr>& sprs);

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const;

private:
	std::vector<SprPtr> m_sprs;

}; // EditableSpriteAOP

}

#endif // _EASYEDITOR_EDITABLE_SPRITE_AOP_H_