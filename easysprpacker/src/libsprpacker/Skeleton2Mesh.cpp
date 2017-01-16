#include "Skeleton2Mesh.h"
#include "PackCoords.h"
#include "PackArray.h"
#include "to_int.h"

#include <bimp/typedef.h>
#include <sprite2/Skeleton2Mesh.h>
#include <simp/Skeleton2Mesh.h>

namespace esprpacker
{

Skeleton2Mesh::Skeleton2Mesh(const s2::Skeleton2Mesh* mesh)
{
	mesh->AddReference();
	m_mesh = mesh;
}

Skeleton2Mesh::~Skeleton2Mesh()
{
	m_mesh->RemoveReference();
}

int Skeleton2Mesh::Type() const
{
	return s2::MESH_SKELETON2;
}

void Skeleton2Mesh::PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	const std::vector<s2::Skeleton2Mesh::Item>& items = m_mesh->GetItems();
	const std::vector<s2::Skeleton2Mesh::Vertex>& vertices = m_mesh->GetVertices();
	lua::assign_with_end(gen, "vertices_num", vertices.size());
	{
		lua::TableAssign ta(gen, "vertices", true);
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			PackToLuaString(items, vertices[i], gen);
		}
	}

	PackCoords::PackToLua(gen, m_mesh->GetTexcoords(), "texcoords", 8192);
	PackArray<int, uint16_t, uint16_t>::PackToLua(gen, m_mesh->GetTriangles(), "triangles");
}

int Skeleton2Mesh::SizeOfUnpackFromBin() const
{
	int sz = simp::Skeleton2Mesh::Size();
	
	// vertices
	const std::vector<s2::Skeleton2Mesh::Vertex>& vertices = m_mesh->GetVertices();
	// items_n
	sz += ALIGN_4BYTE(sizeof(uint8_t) * vertices.size());
	// items
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		sz += SizeOfUnpackFromBin(vertices[i]);
	}

	sz += PackCoords::SizeOfUnpackFromBin(m_mesh->GetTexcoords());
	sz += PackArray<int, uint16_t, uint16_t>::SizeOfUnpackFromBin(m_mesh->GetTriangles());

	return sz;
}

int Skeleton2Mesh::SizeOfPackToBin() const
{
	int sz = 0;

	const std::vector<s2::Skeleton2Mesh::Vertex>& vertices = m_mesh->GetVertices();
	sz += sizeof(uint16_t);						// vertices num
	sz += sizeof(uint8_t) * vertices.size();	// items num
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		sz += SizeOfPackToBin(vertices[i]);
	}

	sz += PackCoords::SizeOfPackToBin(m_mesh->GetTexcoords());
	sz += PackArray<int, uint16_t, uint16_t>::SizeOfPackToBin(m_mesh->GetTriangles());

	return sz;
}

void Skeleton2Mesh::PackToBin(uint8_t** ptr) const
{
	const std::vector<s2::Skeleton2Mesh::Item>& items = m_mesh->GetItems();
	const std::vector<s2::Skeleton2Mesh::Vertex>& vertices = m_mesh->GetVertices();
	// vertices num
	uint16_t v_num = vertices.size();
	pack(v_num, ptr);
	// items num
	for (int i = 0; i < v_num; ++i) {
		uint8_t n = vertices[i].items.size();
		pack(n, ptr);
	}
	// items
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		PackToBin(items, vertices[i], ptr);
	}

	PackCoords::PackToBin(m_mesh->GetTexcoords(), ptr, 8192);
	PackArray<int, uint16_t, uint16_t>::PackToBin(m_mesh->GetTriangles(), ptr);
}

void Skeleton2Mesh::PackToLuaString(const std::vector<s2::Skeleton2Mesh::Item>& items, 
									const s2::Skeleton2Mesh::Vertex& vertex, ebuilder::CodeGenerator& gen)
{
	lua::TableAssign ta(gen, "items", true);
	for (int i = 0, n = vertex.items.size(); i < n; ++i) 
	{
		const s2::Skeleton2Mesh::Item& v = items[vertex.items[i]];
		lua::connect(gen, 4, 
			lua::assign("joint", v.joint), 
			lua::assign("vx", v.vertex.x),
			lua::assign("vy", v.vertex.y),
			lua::assign("weight", v.weight));
	}
}

int Skeleton2Mesh::SizeOfUnpackFromBin(const s2::Skeleton2Mesh::Vertex& vertex)
{
	return simp::Skeleton2Mesh::ItemSize() * vertex.items.size();
}

int Skeleton2Mesh::SizeOfPackToBin(const s2::Skeleton2Mesh::Vertex& vertex)
{
	int sz = 0;
	sz += sizeof(uint16_t) * 4 * vertex.items.size();
	return sz;
}

void Skeleton2Mesh::PackToBin(const std::vector<s2::Skeleton2Mesh::Item>& items, 
							  const s2::Skeleton2Mesh::Vertex& vertex, uint8_t** ptr)
{
	for (int i = 0, n = vertex.items.size(); i < n; ++i) 
	{
		const s2::Skeleton2Mesh::Item& item = items[vertex.items[i]];
		uint16_t joint = item.joint;
		pack(joint, ptr);
		uint16_t vx = float2int(item.vertex.x, 128);
		pack(vx, ptr);
		uint16_t vy = float2int(item.vertex.y, 128);
		pack(vy, ptr);
		uint16_t weight = float2int(item.weight, 4096);
		pack(weight, ptr);
	}
}

}