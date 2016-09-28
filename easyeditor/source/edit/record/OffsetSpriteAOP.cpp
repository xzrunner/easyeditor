#include "OffsetSpriteAOP.h"
#include "Sprite.h"

namespace ee
{

OffsetSpriteAOP::OffsetSpriteAOP(Sprite* spr, 
								 const sm::vec2& new_offset, 
								 const sm::vec2& old_offset)
	: m_spr(spr)
	, m_new_offset(new_offset)
	, m_old_offset(old_offset)
{
}

void OffsetSpriteAOP::Undo()
{
	m_spr->SetOffset(m_old_offset);
}

void OffsetSpriteAOP::Redo()
{
	m_spr->SetOffset(m_new_offset);
}

void OffsetSpriteAOP::Copy(const std::vector<ee::Sprite*>& sprs)
{
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->SetOffset(m_new_offset);
	}
}

// todo
Json::Value OffsetSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
{
	Json::Value ret;
	return ret;
}

}