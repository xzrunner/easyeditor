#include "PerspectiveSpriteAOP.h"

namespace d2d
{

PerspectiveSpriteAOP::PerspectiveSpriteAOP(ISprite* sprite, 
										   const Vector& new_persp, 
										   const Vector& old_persp)
	: m_spr(sprite)
	, m_new_persp(m_new_persp)
	, m_old_persp(m_old_persp)
{
}

void PerspectiveSpriteAOP::undo()
{
	m_spr->SetPerspective(m_old_persp);
}

void PerspectiveSpriteAOP::redo()
{
	m_spr->SetPerspective(m_new_persp);
}

// todo
Json::Value PerspectiveSpriteAOP::store(const std::vector<ISprite*>& sprites)
{
	Json::Value ret;
	return ret;
}

}