#ifndef _EASYSPRPACKER_PACK_MESH_H_
#define _EASYSPRPACKER_PACK_MESH_H_

#include "PackNode.h"

#include <sprite2/MeshSymbol.h>

#include <vector>

namespace emesh { class Symbol; }

namespace esprpacker
{

class MeshShape;

class PackMesh : public PackNode
{
public:
	PackMesh(const std::shared_ptr<emesh::Symbol>& sym);
	~PackMesh();

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp) const override;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const override;
	virtual int SizeOfPackToBin() const override;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const override;	

private:
	const PackNode* m_base;

	const MeshShape* m_mesh;

}; // PackMesh

}

#endif // _EASYSPRPACKER_PACK_MESH_H_