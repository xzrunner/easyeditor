#include "TrianglesMesh.h"
#include "PackCoords.h"
#include "PackArray.h"

#include <sprite2/TrianglesMesh.h>
#include <simp/TrianglesMesh.h>

namespace esprpacker
{

TrianglesMesh::TrianglesMesh(const s2::TrianglesMesh* mesh)
{
	mesh->AddReference();
	m_mesh = mesh;
}

TrianglesMesh::~TrianglesMesh()
{
	m_mesh->RemoveReference();
}

int TrianglesMesh::Type() const
{
	return s2::MESH_TRIANGLES;
}

void TrianglesMesh::PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	PackCoords::PackToLua(gen, m_mesh->GetVertices(), "vertices");
	PackCoords::PackToLua(gen, m_mesh->GetTexcoords(), "texcoords", 8192);
	PackArray<int, uint16_t, uint16_t>::PackToLua(gen, m_mesh->GetTriangles(), "triangles");
}

int TrianglesMesh::SizeOfUnpackFromBin() const
{
	int sz = simp::TrianglesMesh::Size();
	sz += PackCoords::SizeOfUnpackFromBin(m_mesh->GetVertices());
	sz += PackCoords::SizeOfUnpackFromBin(m_mesh->GetTexcoords());
	sz += PackArray<int, uint16_t, uint16_t>::SizeOfUnpackFromBin(m_mesh->GetTriangles());
	return sz;
}

int TrianglesMesh::SizeOfPackToBin() const
{
	int sz = 0;
	sz += PackCoords::SizeOfPackToBin(m_mesh->GetVertices());
	sz += PackCoords::SizeOfPackToBin(m_mesh->GetTexcoords());
	sz += PackArray<int, uint16_t, uint16_t>::SizeOfPackToBin(m_mesh->GetTriangles());
	return sz;
}

void TrianglesMesh::PackToBin(uint8_t** ptr) const
{
	PackCoords::PackToBin(m_mesh->GetVertices(), ptr);
	PackCoords::PackToBin(m_mesh->GetTexcoords(), ptr, 8192);
	PackArray<int, uint16_t, uint16_t>::PackToBin(m_mesh->GetTriangles(), ptr);
}

}