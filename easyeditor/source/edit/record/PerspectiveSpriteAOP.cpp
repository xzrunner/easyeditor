#include "PerspectiveSpriteAOP.h"
#include "Sprite.h"

namespace ee
{

PerspectiveSpriteAOP::PerspectiveSpriteAOP(Sprite* spr, 
										   const sm::vec2& new_persp, 
										   const sm::vec2& old_persp)
	: m_spr(spr)
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
Json::Value PerspectiveSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
{
	Json::Value ret;
	return ret;
}

}