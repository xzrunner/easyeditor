#include "PackMesh.h"

#include "MeshToLuaString.h"
#include "MeshFromLua.h"

#include "MeshToBin.h"
#include "MeshFromBin.h"

namespace erespacker
{

PackMesh::PackMesh(int id)
	: IPackNode(id)
{
}

void PackMesh::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	MeshToLuaString::Pack(this, gen);
}

void PackMesh::UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images)
{
	MeshFromLua::Unpack(L, this);
}

int PackMesh::SizeOfPackToBin() const
{
	return MeshToBin::Size(this);
}

void PackMesh::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	MeshToBin::Pack(this, ptr);
}

int PackMesh::SizeOfUnpackFromBin() const
{
	return MeshFromBin::Size(this);
}

void PackMesh::UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images)
{
	MeshFromBin::Unpack(ptr, this);
}

}