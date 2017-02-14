#include "TrianglesMesh.h"
#include "PackCoords.h"
#include "PackArray.h"

#include <simp/TrianglesMesh.h>
#include <polymesh/MeshType.h>
#include <polymesh/TrianglesHelper.h>

namespace esprpacker
{

TrianglesMesh::TrianglesMesh(const pm::Triangles* mesh)
	: m_mesh(mesh)
{
}

int TrianglesMesh::Type() const
{
	return pm::MESH_TRIANGLES;
}

void TrianglesMesh::PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	pm::TrianglesHelper::Dump(*m_mesh, vertices, texcoords, triangles);

	PackCoords::PackToLua(gen, vertices, "vertices");
	PackCoords::PackToLua(gen, texcoords, "texcoords", 8192);
	PackArray<int, uint16_t, uint16_t>::PackToLua(gen, triangles, "triangles");
}

int TrianglesMesh::SizeOfUnpackFromBin() const
{
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	pm::TrianglesHelper::Dump(*m_mesh, vertices, texcoords, triangles);

	int sz = simp::TrianglesMesh::Size();
	sz += PackCoords::SizeOfUnpackFromBin(vertices);
	sz += PackCoords::SizeOfUnpackFromBin(texcoords);
	sz += PackArray<int, uint16_t, uint16_t>::SizeOfUnpackFromBin(triangles);
	return sz;
}

int TrianglesMesh::SizeOfPackToBin() const
{
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	pm::TrianglesHelper::Dump(*m_mesh, vertices, texcoords, triangles);

	int sz = 0;
	sz += PackCoords::SizeOfPackToBin(vertices);
	sz += PackCoords::SizeOfPackToBin(texcoords);
	sz += PackArray<int, uint16_t, uint16_t>::SizeOfPackToBin(triangles);
	return sz;
}

void TrianglesMesh::PackToBin(uint8_t** ptr) const
{
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	pm::TrianglesHelper::Dump(*m_mesh, vertices, texcoords, triangles);

	PackCoords::PackToBin(vertices, ptr);
	PackCoords::PackToBin(texcoords, ptr, 8192);
	PackArray<int, uint16_t, uint16_t>::PackToBin(triangles, ptr);
}

}