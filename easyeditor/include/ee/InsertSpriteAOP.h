#ifndef _EASYEDITOR_INSERT_SPRITE_AOP_H_
#define _EASYEDITOR_INSERT_SPRITE_AOP_H_

#include "AtomicOP.h"

namespace ee
{

class InsertSpriteAOP : public AtomicOP
{
public:
	InsertSpriteAOP(const std::vector<Sprite*>& sprites);
	virtual ~InsertSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	std::vector<Sprite*> m_sprites;

}; // InsertSpriteAOP

}

#endif // _EASYEDITOR_INSERT_SPRITE_AOP_H_