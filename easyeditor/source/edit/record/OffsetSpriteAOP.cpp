#include "OffsetSpriteAOP.h"
#include "Sprite.h"

namespace ee
{

OffsetSpriteAOP::OffsetSpriteAOP(Sprite* sprite, 
								 const sm::vec2& new_offset, 
								 const sm::vec2& old_offset)
	: m_sprite(sprite)
	, m_new_offset(new_offset)
	, m_old_offset(old_offset)
{
}

void OffsetSpriteAOP::Undo()
{
	m_sprite->SetOffset(m_old_offset);
}

void OffsetSpriteAOP::Redo()
{
	m_sprite->SetOffset(m_new_offset);
}

// todo
Json::Value OffsetSpriteAOP::Store(const std::vector<Sprite*>& sprites) const
{
	Json::Value ret;
	return ret;
}

}