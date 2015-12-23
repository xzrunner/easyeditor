#include "PerspectiveSpriteAOP.h"

#include "dataset/ISprite.h"

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

void PerspectiveSpriteAOP::Undo()
{
	m_spr->SetPerspective(m_old_persp);
}

void PerspectiveSpriteAOP::Redo()
{
	m_spr->SetPerspective(m_new_persp);
}

// todo
Json::Value PerspectiveSpriteAOP::Store(const std::vector<ISprite*>& sprites) const
{
	Json::Value ret;
	return ret;
}

}