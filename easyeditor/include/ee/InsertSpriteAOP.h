#ifndef _EASYEDITOR_INSERT_SPRITE_AOP_H_
#define _EASYEDITOR_INSERT_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

namespace ee
{

class InsertSpriteAOP : public AtomicOP
{
public:
	InsertSpriteAOP(const SprPtr& spr);
	InsertSpriteAOP(const std::vector<SprPtr>& sprs);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	std::vector<SprPtr> m_sprs;

}; // InsertSpriteAOP

}

#endif // _EASYEDITOR_INSERT_SPRITE_AOP_H_