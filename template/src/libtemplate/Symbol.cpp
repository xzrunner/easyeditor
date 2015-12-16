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
				  const d2d::ISprite* spr, const d2d::ISprite* root) const
{
}

d2d::Rect Symbol::GetSize(const d2d::ISprite* sprite) const
{
	return d2d::Rect(200, 200);
}

void Symbol::LoadResources()
{
}

}