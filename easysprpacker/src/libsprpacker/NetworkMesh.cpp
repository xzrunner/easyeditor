#include "NetworkMesh.h"
#include "PackCoords.h"

#include <sprite2/NetworkMesh.h>
#include <sprite2/NetworkShape.h>
#include <simp/NetworkMesh.h>

namespace esprpacker
{

NetworkMesh::NetworkMesh(const s2::NetworkMesh* mesh)
{
	mesh->AddReference();
	m_mesh = mesh;
}

NetworkMesh::~NetworkMesh()
{
	m_mesh->RemoveReference();
}

int NetworkMesh::Type() const
{
	return s2::MESH_NETWORK;
}

void NetworkMesh::PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	PackCoords::PackToLua(gen, m_mesh->GetShape()->GetVertices(), "outer");
	PackCoords::PackToLua(gen, m_mesh->GetShape()->GetInnerVertices(), "inner");
}

int NetworkMesh::SizeOfUnpackFromBin() const
{
	int sz = simp::NetworkMesh::Size();
	sz += PackCoords::SizeOfUnpackFromBin(m_mesh->GetShape()->GetVertices());
	sz += PackCoords::SizeOfUnpackFromBin(m_mesh->GetShape()->GetInnerVertices());
	return sz;
}

int NetworkMesh::SizeOfPackToBin() const
{
	int sz = 0;
	sz += PackCoords::SizeOfPackToBin(m_mesh->GetShape()->GetVertices());
	sz += PackCoords::SizeOfPackToBin(m_mesh->GetShape()->GetInnerVertices());
	return sz;
}

void NetworkMesh::PackToBin(uint8_t** ptr) const
{
	PackCoords::PackToBin(m_mesh->GetShape()->GetVertices(), ptr);
	PackCoords::PackToBin(m_mesh->GetShape()->GetInnerVertices(), ptr);
}

}