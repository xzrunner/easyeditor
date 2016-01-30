#ifndef _LIBCOMPLEX_ANCHOR_TO_SPR_H_
#define _LIBCOMPLEX_ANCHOR_TO_SPR_H_

#include <drag2d.h>
#include <easyrespacker.h>

namespace ecomplex
{

class AnchorToSpr
{
public:
	static d2d::Sprite* Trans(const librespacker::PackAnchor* anchor);

}; // AnchorToSpr

}

#endif // _LIBCOMPLEX_ANCHOR_TO_SPR_H_