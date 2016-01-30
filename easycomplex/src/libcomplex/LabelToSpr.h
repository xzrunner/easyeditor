#ifndef _LIBCOMPLEX_LABEL_TO_SPR_H_
#define _LIBCOMPLEX_LABEL_TO_SPR_H_

#include <drag2d.h>
#include <easyrespacker.h>

namespace ecomplex
{

class LabelToSpr
{
public:
	static d2d::Sprite* Trans(const librespacker::PackLabel* label);

}; // LabelToSpr

}

#endif // _LIBCOMPLEX_LABEL_TO_SPR_H_