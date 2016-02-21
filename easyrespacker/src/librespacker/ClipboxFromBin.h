#ifndef _EASYRESPACKER_CLIPBOX_FROM_BIN_H_
#define _EASYRESPACKER_CLIPBOX_FROM_BIN_H_

#include "PackClipbox.h"

namespace erespacker
{

class ClipboxFromBin
{
public:
	static int Size();

	static void Unpack(uint8_t** ptr, PackClipbox* cb);

}; // ClipboxFromBin

}

#endif // _EASYRESPACKER_CLIPBOX_FROM_BIN_H_