#ifndef _EASYCOMPLEX_LABEL_TO_SPR_H_
#define _EASYCOMPLEX_LABEL_TO_SPR_H_


#include <easyrespacker.h>

namespace ecomplex
{

class LabelToSpr
{
public:
	static ee::SprPtr Trans(const erespacker::PackLabel* label);

}; // LabelToSpr

}

#endif // _EASYCOMPLEX_LABEL_TO_SPR_H_