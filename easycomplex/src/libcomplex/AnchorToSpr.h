#ifndef _LIBCOMPLEX_ANCHOR_TO_SPR_H_
#define _LIBCOMPLEX_ANCHOR_TO_SPR_H_

#include <drag2d.h>
#include <easycoco.h>

namespace ecomplex
{

class AnchorToSpr
{
public:
	static d2d::ISprite* Trans(const libcoco::PackAnchor* anchor);

}; // AnchorToSpr

}

#endif // _LIBCOMPLEX_ANCHOR_TO_SPR_H_