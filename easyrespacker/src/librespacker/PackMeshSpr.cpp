#include "PackMeshSpr.h"

#include "MeshSprToLuaString.h"
#include "MeshSprFromLua.h"

#include "MeshSprToBin.h"
#include "MeshSprFromBin.h"

namespace erespacker
{

PackMeshSpr::PackMeshSpr(int id)
	: IPackNode(id)
{
}

void PackMeshSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	MeshSprToLuaString::Pack(this, gen);
}

void PackMeshSpr::UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images)
{
	MeshSprFromLua::Unpack(L, this);
}

int PackMeshSpr::SizeOfPackToBin() const
{
	return MeshSprToBin::Size(this);
}

void PackMeshSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	MeshSprToBin::Pack(this, ptr);
}

int PackMeshSpr::SizeOfUnpackFromBin() const
{
	return MeshSprFromBin::Size(this);
}

void PackMeshSpr::UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images)
{
	MeshSprFromBin::Unpack(ptr, this);
}

}