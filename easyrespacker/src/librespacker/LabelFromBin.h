#ifndef _EASYRESPACKER_LABEL_FROM_BIN_H_
#define _EASYRESPACKER_LABEL_FROM_BIN_H_

#include "PackLabel.h"

namespace erespacker
{

class LabelFromBin
{
public:
	static int Size(const PackLabel* label);

	static void Unpack(uint8_t** ptr, PackLabel* label);

}; // LabelFromBin

}

#endif // _EASYRESPACKER_LABEL_FROM_BIN_H_