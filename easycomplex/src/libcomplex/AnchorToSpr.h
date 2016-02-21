#ifndef _EASYCOMPLEX_ANCHOR_TO_SPR_H_
#define _EASYCOMPLEX_ANCHOR_TO_SPR_H_


#include <easyrespacker.h>

namespace ecomplex
{

class AnchorToSpr
{
public:
	static ee::Sprite* Trans(const erespacker::PackAnchor* anchor);

}; // AnchorToSpr

}

#endif // _EASYCOMPLEX_ANCHOR_TO_SPR_H_