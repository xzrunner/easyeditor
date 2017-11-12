#ifndef _EASYSPRPACKER_TRIANGLES_MESH_H_
#define _EASYSPRPACKER_TRIANGLES_MESH_H_

#include "MeshShape.h"

namespace pm { class Triangles; }

namespace esprpacker
{

class TrianglesMesh : public MeshShape
{
public:
	TrianglesMesh(const pm::Triangles* mesh);

	virtual int Type() const override;

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen) const;

	virtual int SizeOfUnpackFromBin() const override;

	virtual int SizeOfPackToBin() const override;
	virtual void PackToBin(uint8_t** ptr) const;

private:
	const pm::Triangles* m_mesh;

}; // TrianglesMesh

}

#endif // _EASYSPRPACKER_TRIANGLES_MESH_H_