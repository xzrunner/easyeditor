#ifndef _EASYRESPACKER_PACK_MESH_H_
#define _EASYRESPACKER_PACK_MESH_H_

#include "IPackNode.h"

#include <SM_Vector.h>

namespace erespacker
{

class PackMesh : public IPackNode
{
public:
	const IPackNode* base;

	std::vector<sm::vec2> triangles;
	
public:	
	PackMesh() {}
	PackMesh(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::Image*>& images);

}; // PackMesh

}

#endif // _EASYRESPACKER_PACK_MESH_H_