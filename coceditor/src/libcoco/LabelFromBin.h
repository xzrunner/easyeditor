#ifndef _LIBCOCO_LABEL_FROM_BIN_H_
#define _LIBCOCO_LABEL_FROM_BIN_H_

#include "PackLabel.h"

namespace libcoco
{

class LabelFromBin
{
public:
	static void Unpack(uint8_t** ptr, PackLabel* label);

}; // LabelFromBin

}

#endif // _LIBCOCO_LABEL_FROM_BIN_H_