#ifndef _EASYSPRPACKER_PACK_MESH_SPR_H_
#define _EASYSPRPACKER_PACK_MESH_SPR_H_

#include "PackNode.h"

#include <SM_Vector.h>

#include <vector>

namespace emesh { class Sprite; }

namespace esprpacker
{

class PackMeshSpr : public PackNode
{
public:
	PackMeshSpr(const emesh::Sprite* spr);
	virtual ~PackMeshSpr();

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
		float scale) const;

	bool Equal(const emesh::Sprite* spr) const;

private:
	const PackNode *m_mesh, *m_base;
	std::vector<sm::vec2> m_trans_pairs;

}; // PackMeshSpr

}

#endif // _EASYSPRPACKER_PACK_MESH_SPR_H_