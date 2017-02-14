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
	PackCoords::PackToLua(gen, m_mesh->GetVertices(), "outline");
	
	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	PackCoords::PackToLua(gen, points, "points");
}

int PointsMesh::SizeOfUnpackFromBin() const
{
	int sz = simp::PointsMesh::Size();
	sz += PackCoords::SizeOfUnpackFromBin(m_mesh->GetVertices());
	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	sz += PackCoords::SizeOfUnpackFromBin(points);
	return sz;
}

int PointsMesh::SizeOfPackToBin() const
{
	int sz = 0;
	sz += PackCoords::SizeOfPackToBin(m_mesh->GetVertices());
	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	sz += PackCoords::SizeOfPackToBin(points);
	return sz;
}

void PointsMesh::PackToBin(uint8_t** ptr) const
{
	PackCoords::PackToBin(m_mesh->GetVertices(), ptr);

	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	PackCoords::PackToBin(points, ptr);
}

}