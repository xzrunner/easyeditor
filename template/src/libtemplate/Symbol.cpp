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

void Symbol::Draw(const ee::Matrix& mt, const ee::ColorTrans& color, 
				  const ee::Sprite* spr, const ee::Sprite* root) const
{
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return ee::Rect(200, 200);
}

void Symbol::LoadResources()
{
}

}