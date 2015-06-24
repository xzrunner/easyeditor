#include "Symbol.h"
#include "Sprite.h"

namespace eanim3d
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

void Symbol::reloadTexture() const
{
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
	return d2d::Rect(200, 200);
}

void Symbol::loadResources()
{
}

}