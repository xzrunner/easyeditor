#ifndef _EASYEDITOR_VISIBLE_SPRITE_AOP_H_
#define _EASYEDITOR_VISIBLE_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

namespace ee
{

class VisibleSpriteAOP : public AtomicOP
{
public:
	VisibleSpriteAOP(const std::vector<SprPtr>& sprs);

	virtual void Undo();
	virtual void Redo();
	virtual void Copy(const std::vector<SprPtr>& sprs);

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const;

private:
	std::vector<SprPtr> m_sprs;

}; // VisibleSpriteAOP

}

#endif // _EASYEDITOR_VISIBLE_SPRITE_AOP_H_