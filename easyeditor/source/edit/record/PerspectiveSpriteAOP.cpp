#include "PerspectiveSpriteAOP.h"
#include "Sprite.h"

namespace ee
{

PerspectiveSpriteAOP::PerspectiveSpriteAOP(Sprite* sprite, 
										   const sm::vec2& new_persp, 
										   const sm::vec2& old_persp)
	: m_spr(sprite)
	, m_new_persp(m_new_persp)
	, m_old_persp(m_old_persp)
{
}

void PerspectiveSpriteAOP::Undo()
{
	m_spr->SetPerspective(m_old_persp);
}

void PerspectiveSpriteAOP::Redo()
{
	m_spr->SetPerspective(m_new_persp);
}

// todo
Json::Value PerspectiveSpriteAOP::Store(const std::vector<Sprite*>& sprites) const
{
	Json::Value ret;
	return ret;
}

}