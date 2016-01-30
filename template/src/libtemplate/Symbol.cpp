#include "Symbol.h"
#include "Sprite.h"

namespace etemplate
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

void Symbol::ReloadTexture() const
{
}

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::Sprite* spr, const d2d::Sprite* root) const
{
}

d2d::Rect Symbol::GetSize(const d2d::Sprite* sprite) const
{
	return d2d::Rect(200, 200);
}

void Symbol::LoadResources()
{
}

}