#ifndef _LIBCOCO_PACK_TO_BIN_H_
#define _LIBCOCO_PACK_TO_BIN_H_

#include <drag2d.h>

#include "tools.h"

namespace libcoco
{

class PackToBin
{
public:
	static void Pack(const std::string& filepath, const d2d::TexturePacker& tp,
		bool compress, TextureType type);

private:
	static void PackEPE(const std::string& filepath, const d2d::TexturePacker& tp,
		bool compress);	

	static void PackEPT(const std::string& filepath, const d2d::TexturePacker& tp,
		TextureType type);	

}; // PackToBin

}

#endif // _LIBCOCO_PACK_TO_BIN_H_