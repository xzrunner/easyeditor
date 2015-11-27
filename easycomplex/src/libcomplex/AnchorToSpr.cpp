#include "AnchorToSpr.h"

namespace ecomplex
{

d2d::ISprite* AnchorToSpr::Trans(const librespacker::PackAnchor* anchor)
{
	d2d::FontBlankSymbol* symbol = new d2d::FontBlankSymbol();

	d2d::FontBlankSprite* spr = new d2d::FontBlankSprite(symbol);
	spr->width = 32;
	spr->height = 32;
	return spr;
}

}