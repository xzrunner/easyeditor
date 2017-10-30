#ifndef _EASYSPRPACKER_PACK_ARRAY_H_
#define _EASYSPRPACKER_PACK_ARRAY_H_

#include <cu/cu_stl.h>

#include <vector>

#include <stdint.h>

namespace ebuilder { class CodeGenerator; }

namespace esprpacker
{

template<typename T, typename TPack, typename TUnPack>
class PackArray
{
public:
	static void PackToLua(ebuilder::CodeGenerator& gen, const CU_VEC<T>& array, const std::string& name);

	static int SizeOfUnpackFromBin(const CU_VEC<T>& array);
	static int SizeOfPackToBin(const CU_VEC<T>& array);
	static void PackToBin(const CU_VEC<T>& array, uint8_t** ptr);

}; // PackArray

}

#include "PackArray.inl"

#endif // _EASYSPRPACKER_PACK_ARRAY_H_
