#ifndef _LIBCOMPLEX_LABEL_TO_SPR_H_
#define _LIBCOMPLEX_LABEL_TO_SPR_H_


#include <easyrespacker.h>

namespace ecomplex
{

class LabelToSpr
{
public:
	static ee::Sprite* Trans(const librespacker::PackLabel* label);

}; // LabelToSpr

}

#endif // _LIBCOMPLEX_LABEL_TO_SPR_H_