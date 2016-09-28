#ifndef _EASYEDITOR_EDITABLE_SPRITE_AOP_H_
#define _EASYEDITOR_EDITABLE_SPRITE_AOP_H_

#include "AtomicOP.h"

namespace ee
{

class EditableSpriteAOP : public AtomicOP
{
public:
	EditableSpriteAOP(const std::vector<Sprite*>& sprs);
	virtual ~EditableSpriteAOP();

	virtual void Undo();
	virtual void Redo();
	virtual void Copy(const std::vector<Sprite*>& sprs);

	virtual Json::Value Store(const std::vector<Sprite*>& sprs) const;

private:
	std::vector<Sprite*> m_sprs;

}; // EditableSpriteAOP

}

#endif // _EASYEDITOR_EDITABLE_SPRITE_AOP_H_