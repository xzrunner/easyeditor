#ifndef _LIBCOCO_LABEL_TO_BIN_H_
#define _LIBCOCO_LABEL_TO_BIN_H_

#include "PackLabel.h"

namespace libcoco
{

class LabelToBin
{
public:
	static int Size();

	static void Pack(const PackLabel* label, uint8_t** ptr);

}; // LabelToBin

}

#endif // _LIBCOCO_LABEL_TO_BIN_H_