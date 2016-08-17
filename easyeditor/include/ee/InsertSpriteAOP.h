#ifndef _EASYEDITOR_INSERT_SPRITE_AOP_H_
#define _EASYEDITOR_INSERT_SPRITE_AOP_H_

#include "AtomicOP.h"

namespace ee
{

class InsertSpriteAOP : public AtomicOP
{
public:
	InsertSpriteAOP(Sprite* spr);
	InsertSpriteAOP(const std::vector<Sprite*>& sprs);
	virtual ~InsertSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprs) const;

private:
	std::vector<Sprite*> m_sprs;

}; // InsertSpriteAOP

}

#endif // _EASYEDITOR_INSERT_SPRITE_AOP_H_