
#include "ChangeImage.h"

using namespace ebuilder::preview;

ChangeImage::ChangeImage(d2d::ISprite* sprite, d2d::ISymbol* symbol)
	: m_sprite(sprite)
	, m_symbol(symbol)
{
}

void ChangeImage::trigger()
{
	m_sprite->setSymbol(m_symbol);
}
