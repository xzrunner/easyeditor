#ifndef _EASYSPRPACKER_SKIN2_MESH_H_
#define _EASYSPRPACKER_SKIN2_MESH_H_

#include "MeshShape.h"

namespace pm { class Skin2Triangles; }

namespace esprpacker
{

class Skin2Mesh : public MeshShape
{
public:
	Skin2Mesh(const pm::Skin2Triangles* mesh);

	virtual int Type() const;

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen) const;

	virtual int SizeOfUnpackFromBin() const;

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr) const;

private:
	const pm::Skin2Triangles* m_mesh;

}; // Skin2Mesh

}

#endif // _EASYSPRPACKER_SKIN2_MESH_H_