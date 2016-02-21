#ifndef _EASYRESPACKER_PACK_TO_BIN_H_
#define _EASYRESPACKER_PACK_TO_BIN_H_

#include "typedef.h"

#include <string>

namespace ee { class TexturePacker; }

namespace erespacker
{

class PackToBin
{
public:
	static void PackEPE(const std::string& filepath, const ee::TexturePacker& tp,
		bool compress, float scale);	

	static void PackEPT(const std::string& filepath, const ee::TexturePacker& tp,
		TextureType type, int LOD, float scale);	

}; // PackToBin

}

#endif // _EASYRESPACKER_PACK_TO_BIN_H_