#ifndef _LIBCOMPLEX_ANCHOR_TO_SPR_H_
#define _LIBCOMPLEX_ANCHOR_TO_SPR_H_


#include <easyrespacker.h>

namespace ecomplex
{

class AnchorToSpr
{
public:
	static ee::Sprite* Trans(const librespacker::PackAnchor* anchor);

}; // AnchorToSpr

}

#endif // _LIBCOMPLEX_ANCHOR_TO_SPR_H_