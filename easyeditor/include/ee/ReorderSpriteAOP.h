#ifndef _EASYEDITOR_REORDER_SPRITE_AOP_H_
#define _EASYEDITOR_REORDER_SPRITE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

namespace ee
{

class ReorderSpriteAOP : public AtomicOP
{
public:
	ReorderSpriteAOP(const std::vector<SprPtr>& sprs, bool up);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	void Reorder(bool up);

private:
	std::vector<SprPtr> m_sprs;
	bool m_up;

}; // ReorderSpriteAOP

}

#endif // _EASYEDITOR_REORDER_SPRITE_AOP_H_