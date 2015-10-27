#ifndef _LIBRESPACKER_CLIPBOX_TO_BIN_H_
#define _LIBRESPACKER_CLIPBOX_TO_BIN_H_

#include "PackClipbox.h"

namespace librespacker
{

class ClipboxToBin
{
public:
	static int Size();

	static void Pack(const PackClipbox* label, uint8_t** ptr);

}; // ClipboxToBin

}

#endif // _LIBRESPACKER_CLIPBOX_TO_BIN_H_