#include "AnchorToSpr.h"

namespace ecomplex
{

d2d::ISprite* AnchorToSpr::Trans(const libcoco::PackAnchor* anchor)
{
	d2d::FontBlankSymbol* symbol = new d2d::FontBlankSymbol();

	d2d::FontSprite* spr = new d2d::FontSprite(symbol);
	spr->width = 32;
	spr->height = 32;
	return spr;
}

}