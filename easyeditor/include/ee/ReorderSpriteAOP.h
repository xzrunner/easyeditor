#ifndef _EASYEDITOR_REORDER_SPRITE_AOP_H_
#define _EASYEDITOR_REORDER_SPRITE_AOP_H_

#include "AtomicOP.h"

namespace ee
{

class ReorderSpriteAOP : public AtomicOP
{
public:
	ReorderSpriteAOP(const std::vector<Sprite*>& sprs, bool up);
	virtual ~ReorderSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprs) const;

private:
	void Reorder(bool up);

private:
	std::vector<Sprite*> m_sprs;
	bool m_up;

}; // ReorderSpriteAOP

}

#endif // _EASYEDITOR_REORDER_SPRITE_AOP_H_