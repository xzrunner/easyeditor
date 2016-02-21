#include "AnchorToSpr.h"

#include <ee/FontBlankSymbol.h>
#include <ee/FontBlankSprite.h>

namespace ecomplex
{

ee::Sprite* AnchorToSpr::Trans(const erespacker::PackAnchor* anchor)
{
	ee::FontBlankSymbol* symbol = new ee::FontBlankSymbol();

	ee::FontBlankSprite* spr = new ee::FontBlankSprite(symbol);
	spr->width = 32;
	spr->height = 32;
	return spr;
}

}