#ifndef _LIBRESPACKER_PACK_TO_BIN_H_
#define _LIBRESPACKER_PACK_TO_BIN_H_

#include <drag2d.h>

#include "typedef.h"

namespace librespacker
{

class PackToBin
{
public:
	static void PackEPE(const std::string& filepath, const d2d::TexturePacker& tp,
		bool compress, float scale);	

	static void PackEPT(const std::string& filepath, const d2d::TexturePacker& tp,
		TextureType type, int LOD, float scale);	

}; // PackToBin

}

#endif // _LIBRESPACKER_PACK_TO_BIN_H_