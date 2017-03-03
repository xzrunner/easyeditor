#include "Skin2Mesh.h"
#include "PackCoords.h"
#include "PackArray.h"
#include "to_int.h"

#include <bimp/bimp_typedef.h>
#include <simp/SIMP_Skin2Mesh.h>
#include <polymesh/Skin2Triangles.h>
#include <polymesh/MeshType.h>

namespace esprpacker
{

Skin2Mesh::Skin2Mesh(const pm::Skin2Triangles* mesh)
	: m_mesh(mesh)
{
}

int Skin2Mesh::Type() const
{
	return pm::MESH_SKIN2;
}

void Skin2Mesh::PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	// joints
	lua::assign_with_end(gen, "joints_num", m_mesh->joint_num);
	{
		lua::TableAssign ta(gen, "joints", true);
		for (int i = 0; i < m_mesh->joint_num; ++i) 
		{
			const pm::Skin2Joint& joint = m_mesh->joints[i];
			lua::connect(gen, 6, 
				lua::assign("joint", joint.joint), 
				lua::assign("x", joint.vertex.x),
				lua::assign("y", joint.vertex.y),
				lua::assign("dx", joint.offset.x),
				lua::assign("dy", joint.offset.y),
				lua::assign("weight", joint.weight));
		}
	}
	// vertices
	lua::assign_with_end(gen, "vertices_num", m_mesh->vert_num);
	{
		lua::TableAssign ta(gen, "vertices", true);
		for (int i = 0; i < m_mesh->vert_num; ++i) 
		{
			const pm::Skin2Vertex& vertex = m_mesh->vertices[i];
			lua::connect(gen, 5, 
				lua::assign("joint_count", m_mesh->vertices_joint[i]), 
				lua::assign("x", vertex.xy.x), 
				lua::assign("y", vertex.xy.y),
				lua::assign("u", vertex.uv.x), 
				lua::assign("v", vertex.uv.y));
		}
	}
	// triangles
	std::vector<int> triangles;
	triangles.reserve(m_mesh->tri_num);
	for (int i = 0; i < m_mesh->tri_num; ++i) {
		triangles.push_back(m_mesh->triangles[i]);
	}
	PackArray<int, uint16_t, uint16_t>::PackToLua(gen, triangles, "triangles");
}

int Skin2Mesh::SizeOfUnpackFromBin() const
{
	int sz = simp::Skin2Mesh::Size();
	
	// joints_n
	sz += ALIGN_4BYTE(sizeof(uint8_t) * m_mesh->vert_num);
	// joints
	sz += simp::Skin2Mesh::JointSize() * m_mesh->joint_num;
	// texcoords
	sz += sizeof(int16_t) * 2 * m_mesh->vert_num;
	// triangles
	sz += sizeof(uint16_t) * m_mesh->tri_num;

	return sz;
}

int Skin2Mesh::SizeOfPackToBin() const
{
	int sz = 0;

	// vertices num
	sz += sizeof(uint16_t);
	// joints num
	sz += sizeof(uint8_t) * m_mesh->vert_num;
	// joints
	sz += sizeof(uint16_t) * 4 * m_mesh->joint_num;
	// texcoords
	sz += sizeof(uint16_t);
	sz += sizeof(int16_t) * 2 * m_mesh->vert_num;
	// triangles
	sz += sizeof(uint16_t);
	sz += sizeof(uint16_t) * m_mesh->tri_num;

	return sz;
}

void Skin2Mesh::PackToBin(uint8_t** ptr) const
{
	// vertices num
	uint16_t v_num = m_mesh->vert_num;
	pack(v_num, ptr);
	// joints num
	for (int i = 0; i < v_num; ++i) {
		uint8_t n = m_mesh->vertices_joint[i];
		pack(n, ptr);
	}
	// joints
	for (int i = 0; i < m_mesh->joint_num; ++i) 
	{
		const pm::Skin2Joint& joint = m_mesh->joints[i];
		uint16_t j = joint.joint;
		pack(j, ptr);
		uint16_t vx = float2int(joint.vertex.x, 128);
		pack(vx, ptr);
		uint16_t vy = float2int(joint.vertex.y, 128);
		pack(vy, ptr);
		uint16_t weight = float2int(joint.weight, 4096);
		pack(weight, ptr);
	}
	// texcoords
	std::vector<sm::vec2> texcoords;
	texcoords.reserve(m_mesh->vert_num);
	for (int i = 0; i < m_mesh->vert_num; ++i) {
		texcoords.push_back(m_mesh->vertices[i].uv);
	}
	PackCoords::PackToBin(texcoords, ptr, 8192);
	// triangles
	std::vector<int> triangles;
	triangles.reserve(m_mesh->tri_num);
	for (int i = 0; i < m_mesh->tri_num; ++i) {
		triangles.push_back(m_mesh->triangles[i]);
	}
	PackArray<int, uint16_t, uint16_t>::PackToBin(triangles, ptr);
}

}