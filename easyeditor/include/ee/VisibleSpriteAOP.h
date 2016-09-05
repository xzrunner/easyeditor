#ifndef _EASYEDITOR_VISIBLE_SPRITE_AOP_H_
#define _EASYEDITOR_VISIBLE_SPRITE_AOP_H_

#include "AtomicOP.h"

namespace ee
{

class VisibleSpriteAOP : public AtomicOP
{
public:
	VisibleSpriteAOP(const std::vector<Sprite*>& sprs);
	virtual ~VisibleSpriteAOP();

	virtual void Undo();
	virtual void Redo();
	virtual void Redo(const std::vector<Sprite*>& sprs);

	virtual Json::Value Store(const std::vector<Sprite*>& sprs) const;

private:
	std::vector<Sprite*> m_sprs;

}; // VisibleSpriteAOP

}

#endif // _EASYEDITOR_VISIBLE_SPRITE_AOP_H_