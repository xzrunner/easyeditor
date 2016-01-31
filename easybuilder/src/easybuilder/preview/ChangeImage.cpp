
#include "ChangeImage.h"

using namespace ebuilder::preview;

ChangeImage::ChangeImage(ee::ISprite* sprite, ee::ISymbol* symbol)
	: m_sprite(sprite)
	, m_symbol(symbol)
{
}

void ChangeImage::trigger()
{
	m_sprite->setSymbol(m_symbol);
}
