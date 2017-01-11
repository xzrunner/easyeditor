#ifndef _EASYSPRPACKER_TRIANGLES_MESH_H_
#define _EASYSPRPACKER_TRIANGLES_MESH_H_

#include "MeshShape.h"

namespace s2 { class TrianglesMesh; }

namespace esprpacker
{

class TrianglesMesh : public MeshShape
{
public:
	TrianglesMesh(const s2::TrianglesMesh* mesh);
	virtual ~TrianglesMesh();

	virtual int Type() const;

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen) const;

	virtual int SizeOfUnpackFromBin() const;

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr) const;

private:
	const s2::TrianglesMesh* m_mesh;

}; // TrianglesMesh

}

#endif // _EASYSPRPACKER_TRIANGLES_MESH_H_