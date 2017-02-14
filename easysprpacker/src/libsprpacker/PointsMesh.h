#ifndef _EASYSPRPACKER_POINTS_MESH_H_
#define _EASYSPRPACKER_POINTS_MESH_H_

#include "MeshShape.h"

namespace emesh { class PointsMesh; }

namespace esprpacker
{

class PointsMesh : public MeshShape
{
public:
	PointsMesh(const emesh::PointsMesh* mesh);

	virtual int Type() const;

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen) const;

	virtual int SizeOfUnpackFromBin() const;

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr) const;

private:
	const emesh::PointsMesh* m_mesh;

}; // PointsMesh

}

#endif // _EASYSPRPACKER_POINTS_MESH_H_