#include "PackMeshSpr.h"

namespace erespacker
{

PackMeshSpr::PackMeshSpr(int id)
	: IPackNode(id)
{
}

void PackMeshSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{

}

void PackMeshSpr::UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images)
{

}

int PackMeshSpr::SizeOfPackToBin() const
{
	return 0;
}

void PackMeshSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{

}

int PackMeshSpr::SizeOfUnpackFromBin() const
{
	return 0;
}

void PackMeshSpr::UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images)
{

}

}