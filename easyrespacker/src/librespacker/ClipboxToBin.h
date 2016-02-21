#ifndef _EASYRESPACKER_CLIPBOX_TO_BIN_H_
#define _EASYRESPACKER_CLIPBOX_TO_BIN_H_

#include "PackClipbox.h"

namespace erespacker
{

class ClipboxToBin
{
public:
	static int Size();

	static void Pack(const PackClipbox* label, uint8_t** ptr);

}; // ClipboxToBin

}

#endif // _EASYRESPACKER_CLIPBOX_TO_BIN_H_