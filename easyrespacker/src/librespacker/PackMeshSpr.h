#ifndef _EASYRESPACKER_PACK_MESH_SPR_H_
#define _EASYRESPACKER_PACK_MESH_SPR_H_

#include "IPackNode.h"

#include <SM_Vector.h>
#include <cu/cu_stl.h>

namespace erespacker
{

class PackMeshSpr : public IPackNode
{
public:
	const IPackNode* mesh;

	const IPackNode* base;

	CU_VEC<sm::vec2> trans_pairs;
	
public:	
	PackMeshSpr() {}
	PackMeshSpr(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const override;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::ImagePtr>& images) override;

	virtual int SizeOfPackToBin() const override;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const override;
	virtual int SizeOfUnpackFromBin() const override;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::ImagePtr>& images) override;

}; // PackMeshSpr

}

#endif // _EASYRESPACKER_PACK_MESH_SPR_H_