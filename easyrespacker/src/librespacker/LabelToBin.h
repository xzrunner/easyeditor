#ifndef _LIBRESPACKER_LABEL_TO_BIN_H_
#define _LIBRESPACKER_LABEL_TO_BIN_H_

#include "PackLabel.h"

namespace librespacker
{

class LabelToBin
{
public:
	static int Size(const PackLabel* label);

	static void Pack(const PackLabel* label, uint8_t** ptr);

}; // LabelToBin

}

#endif // _LIBRESPACKER_LABEL_TO_BIN_H_