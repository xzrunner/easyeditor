#include "AnchorToSpr.h"

#include <ee/FontBlankSymbol.h>
#include <ee/FontBlankSprite.h>

namespace ecomplex
{

ee::SprPtr AnchorToSpr::Trans(const erespacker::PackAnchor* anchor)
{
	auto sym = std::make_shared<ee::FontBlankSymbol>();

	auto spr = std::make_shared<ee::FontBlankSprite>(sym);
	spr->width = 32;
	spr->height = 32;
	return spr;
}

}