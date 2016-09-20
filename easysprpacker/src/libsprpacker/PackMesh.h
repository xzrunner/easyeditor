#ifndef _EASYSPRPACKER_PACK_MESH_H_
#define _EASYSPRPACKER_PACK_MESH_H_

#include "PackNode.h"

#include <sprite2/MeshSymbol.h>

#include <vector>

namespace emesh { class Symbol; }

namespace esprpacker
{

class PackMesh : public PackNode
{
public:
	PackMesh(const emesh::Symbol* sym);
	~PackMesh();

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

private:
	const PackNode* m_base;

	// todo: for NetworkMesh
	std::vector<sm::vec2> m_outer_line, m_inner_line;

}; // PackMesh

}

#endif // _EASYSPRPACKER_PACK_MESH_H_