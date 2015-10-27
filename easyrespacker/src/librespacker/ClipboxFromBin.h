#ifndef _LIBRESPACKER_CLIPBOX_FROM_BIN_H_
#define _LIBRESPACKER_CLIPBOX_FROM_BIN_H_

#include "PackClipbox.h"

namespace librespacker
{

class ClipboxFromBin
{
public:
	static int Size();

	static void Unpack(uint8_t** ptr, PackClipbox* cb);

}; // ClipboxFromBin

}

#endif // _LIBRESPACKER_CLIPBOX_FROM_BIN_H_