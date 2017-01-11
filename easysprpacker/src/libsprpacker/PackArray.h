#ifndef _EASYSPRPACKER_PACK_ARRAY_H_
#define _EASYSPRPACKER_PACK_ARRAY_H_

#include <vector>

#include <stdint.h>

namespace ebuilder { class CodeGenerator; }

namespace esprpacker
{

template<typename T, typename TPack, typename TUnPack>
class PackArray
{
public:
	static void PackToLua(ebuilder::CodeGenerator& gen, const std::vector<T>& array, const std::string& name);

	static int SizeOfUnpackFromBin(const std::vector<T>& array);
	static int SizeOfPackToBin(const std::vector<T>& array);
	static void PackToBin(const std::vector<T>& array, uint8_t** ptr);

}; // PackArray

}

#include "PackArray.inl"

#endif // _EASYSPRPACKER_PACK_ARRAY_H_
