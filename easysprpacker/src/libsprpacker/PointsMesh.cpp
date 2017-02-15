#include "PointsMesh.h"
#include "PackCoords.h"

#include <easymesh.h>
#include <simp/PointsMesh.h>
#include <polymesh/MeshType.h>

namespace esprpacker
{

PointsMesh::PointsMesh(const emesh::PointsMesh* mesh)
	: m_mesh(mesh)
{
}

int PointsMesh::Type() const
{
	return pm::MESH_POINTS;
}

void PointsMesh::PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	std::vector<sm::vec2> vertices = m_mesh->GetVertices();
	vertices.pop_back();
	PackCoords::PackToLua(gen, vertices, "outline");
	
	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	PackCoords::PackToLua(gen, points, "points");
}

int PointsMesh::SizeOfUnpackFromBin() const
{
	int sz = simp::PointsMesh::Size();
	std::vector<sm::vec2> vertices = m_mesh->GetVertices();
	vertices.pop_back();
	sz += PackCoords::SizeOfUnpackFromBin(vertices);
	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	sz += PackCoords::SizeOfUnpackFromBin(points);
	return sz;
}

int PointsMesh::SizeOfPackToBin() const
{
	int sz = 0;
	std::vector<sm::vec2> vertices = m_mesh->GetVertices();
	vertices.pop_back();
	sz += PackCoords::SizeOfPackToBin(vertices);
	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	sz += PackCoords::SizeOfPackToBin(points);
	return sz;
}

void PointsMesh::PackToBin(uint8_t** ptr) const
{
	std::vector<sm::vec2> vertices = m_mesh->GetVertices();
	vertices.pop_back();
	PackCoords::PackToBin(vertices, ptr);

	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	PackCoords::PackToBin(points, ptr);
}

}