#ifndef _EASYSPRPACKER_PACK_COORDS_H_
#define _EASYSPRPACKER_PACK_COORDS_H_

#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <vector>

#include <stdint.h>

namespace ebuilder { class CodeGenerator; }

namespace esprpacker
{

class PackCoords
{
public:
	// i16
	static void PackToLua(ebuilder::CodeGenerator& gen, const CU_VEC<sm::vec2>& vertices, const std::string& name, int precision = 16);
	static int SizeOfUnpackFromBin(const CU_VEC<sm::vec2>& vertices);
	static int SizeOfPackToBin(const CU_VEC<sm::vec2>& vertices);
	static void PackToBin(const CU_VEC<sm::vec2>& vertices, uint8_t** ptr, int precision = 16);

	// f32
	static void PackToLuaF32(ebuilder::CodeGenerator& gen, const CU_VEC<sm::vec2>& vertices, const std::string& name);
	static int SizeOfUnpackFromBinF32(const CU_VEC<sm::vec2>& vertices);
	static int SizeOfPackToBinF32(const CU_VEC<sm::vec2>& vertices);
	static void PackToBinF32(const CU_VEC<sm::vec2>& vertices, uint8_t** ptr);

}; // PackCoords

}

#endif // _EASYSPRPACKER_PACK_COORDS_H_
