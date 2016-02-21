#ifndef _EASYRESPACKER_LABEL_TO_BIN_H_
#define _EASYRESPACKER_LABEL_TO_BIN_H_

#include "PackLabel.h"

namespace erespacker
{

class LabelToBin
{
public:
	static int Size(const PackLabel* label);

	static void Pack(const PackLabel* label, uint8_t** ptr);

}; // LabelToBin

}

#endif // _EASYRESPACKER_LABEL_TO_BIN_H_