#include "OffsetSpriteAOP.h"

#include "dataset/ISprite.h"

namespace d2d
{

OffsetSpriteAOP::OffsetSpriteAOP(ISprite* sprite, 
								 const Vector& new_offset, 
								 const Vector& old_offset)
	: m_sprite(sprite)
	, m_new_offset(new_offset)
	, m_old_offset(old_offset)
{
}

void OffsetSpriteAOP::undo()
{
	m_sprite->setOffset(m_old_offset);
}

void OffsetSpriteAOP::redo()
{
	m_sprite->setOffset(m_new_offset);
}

// todo
Json::Value OffsetSpriteAOP::store(const std::vector<ISprite*>& sprites)
{
	Json::Value ret;
	return ret;
}

}