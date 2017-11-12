#ifndef _EASYEDITOR_DELETE_SPRITE_AOP_H_
#define _EASYEDITOR_DELETE_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

namespace ee
{

class DeleteSpriteAOP : public AtomicOP
{
public:
	DeleteSpriteAOP(const std::vector<SprPtr>& sprs);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	std::vector<SprPtr> m_sprs;

}; // DeleteSpriteAOP

}

#endif // _EASYEDITOR_DELETE_SPRITE_AOP_H_