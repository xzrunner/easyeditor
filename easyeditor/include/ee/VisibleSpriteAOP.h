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

	virtual void Undo() override;
	virtual void Redo() override;
	virtual void Copy(const std::vector<SprPtr>& sprs) override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	std::vector<SprPtr> m_sprs;

}; // VisibleSpriteAOP

}

#endif // _EASYEDITOR_VISIBLE_SPRITE_AOP_H_