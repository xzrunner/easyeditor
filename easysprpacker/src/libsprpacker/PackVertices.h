#ifndef _EASYSPRPACKER_PACK_VERTICES_H_
#define _EASYSPRPACKER_PACK_VERTICES_H_

#include <SM_Vector.h>

#include <vector>

#include <stdint.h>

namespace ebuilder { class CodeGenerator; }

namespace esprpacker
{

class PackVertices
{
public:
	static void PackToLua(ebuilder::CodeGenerator& gen, const std::vector<sm::vec2>& vertices, const std::string& name);

	static int SizeOfUnpackFromBin(const std::vector<sm::vec2>& vertices);
	static int SizeOfPackToBin(const std::vector<sm::vec2>& vertices);
	static void PackToBin(const std::vector<sm::vec2>& vertices, uint8_t** ptr);

}; // PackVertices

}

#endif // _EASYSPRPACKER_PACK_VERTICES_H_
