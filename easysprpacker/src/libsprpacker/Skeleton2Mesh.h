#ifndef _EASYSPRPACKER_SKELETON2_MESH_H_
#define _EASYSPRPACKER_SKELETON2_MESH_H_

#include "MeshShape.h"

#include <sprite2/Skeleton2Mesh.h>

namespace esprpacker
{

class Skeleton2Mesh : public MeshShape
{
public:
	Skeleton2Mesh(const s2::Skeleton2Mesh* mesh);
	virtual ~Skeleton2Mesh();

	virtual int Type() const;

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen) const;

	virtual int SizeOfUnpackFromBin() const;

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr) const;

private:
	static void PackToLuaString(const std::vector<s2::Skeleton2Mesh::Item>& items, 
		const s2::Skeleton2Mesh::Vertex& vertex, ebuilder::CodeGenerator& gen);
	static int  SizeOfUnpackFromBin(const s2::Skeleton2Mesh::Vertex& vertex);
	static int  SizeOfPackToBin(const s2::Skeleton2Mesh::Vertex& vertex);
	static void PackToBin(const std::vector<s2::Skeleton2Mesh::Item>& items, 
		const s2::Skeleton2Mesh::Vertex& vertex, uint8_t** ptr);

private:
	const s2::Skeleton2Mesh* m_mesh;

}; // Skeleton2Mesh

}

#endif // _EASYSPRPACKER_SKELETON2_MESH_H_