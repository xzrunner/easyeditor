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
	PackCoords::PackToLua(gen, FormatVertices(), "outline");
	
	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	PackCoords::PackToLua(gen, points, "points");
}

int PointsMesh::SizeOfUnpackFromBin() const
{
	int sz = simp::PointsMesh::Size();
	sz += PackCoords::SizeOfUnpackFromBin(FormatVertices());
	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	sz += PackCoords::SizeOfUnpackFromBin(points);
	return sz;
}

int PointsMesh::SizeOfPackToBin() const
{
	int sz = 0;
	sz += PackCoords::SizeOfPackToBin(FormatVertices());
	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	sz += PackCoords::SizeOfPackToBin(points);
	return sz;
}

void PointsMesh::PackToBin(uint8_t** ptr) const
{
	PackCoords::PackToBin(FormatVertices(), ptr);

	std::vector<sm::vec2> points;
	m_mesh->GetInnerPoints(points);
	PackCoords::PackToBin(points, ptr);
}

std::vector<sm::vec2> PointsMesh::FormatVertices() const
{
	std::vector<sm::vec2> ret = m_mesh->GetVertices();
	if (ret.size() > 1) {
		if (ret.front() == ret.back()) {
			ret.pop_back();
		}
	}
	return ret;
}

}