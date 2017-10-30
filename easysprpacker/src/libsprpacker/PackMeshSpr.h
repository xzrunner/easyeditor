#ifndef _EASYSPRPACKER_PACK_MESH_SPR_H_
#define _EASYSPRPACKER_PACK_MESH_SPR_H_

#include "PackNode.h"

#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <vector>
#include <memory>

namespace emesh { class Sprite; }

namespace esprpacker
{

class PackMeshSpr : public PackNode
{
public:
	PackMeshSpr(const std::shared_ptr<emesh::Sprite>& spr);
	virtual ~PackMeshSpr();

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const;	

	bool Equal(const std::shared_ptr<emesh::Sprite>& spr) const;

private:
	const PackNode *m_mesh, *m_base;
	CU_VEC<std::pair<int, sm::vec2> > m_transform;

}; // PackMeshSpr

}

#endif // _EASYSPRPACKER_PACK_MESH_SPR_H_