#ifndef _EASYSPRPACKER_NETWORK_MESH_H_
#define _EASYSPRPACKER_NETWORK_MESH_H_

#include "MeshShape.h"

namespace s2 { class NetworkMesh; }

namespace esprpacker
{

class NetworkMesh : public MeshShape
{
public:
	NetworkMesh(const s2::NetworkMesh* mesh);
	virtual ~NetworkMesh();

	virtual int Type() const;

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen) const;

	virtual int SizeOfUnpackFromBin() const;

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr) const;

private:
	const s2::NetworkMesh* m_mesh;

}; // NetworkMesh

}

#endif // _EASYSPRPACKER_NETWORK_MESH_H_