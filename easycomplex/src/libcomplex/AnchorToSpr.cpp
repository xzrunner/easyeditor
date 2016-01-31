#include "AnchorToSpr.h"

namespace ecomplex
{

ee::Sprite* AnchorToSpr::Trans(const librespacker::PackAnchor* anchor)
{
	ee::FontBlankSymbol* symbol = new ee::FontBlankSymbol();

	ee::FontBlankSprite* spr = new ee::FontBlankSprite(symbol);
	spr->width = 32;
	spr->height = 32;
	return spr;
}

}