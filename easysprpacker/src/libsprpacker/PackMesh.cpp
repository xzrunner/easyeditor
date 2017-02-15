#include "PackMesh.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "PackCoords.h"

#include "PointsMesh.h"
#include "TrianglesMesh.h"
#include "Skin2Mesh.h"

#include <easymesh.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeMesh.h>
#include <simp/simp_types.h>
#include <simp/MeshType.h>
#include <polymesh/Mesh.h>
#include <polymesh/TrianglesMesh.h>
#include <polymesh/Skin2Mesh.h>

namespace esprpacker
{

PackMesh::PackMesh(const emesh::Symbol* sym)
	: m_mesh(NULL)
{
	m_base = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(
		sym->GetMesh()->GetBaseSymbol()));

	const pm::Mesh* mesh = sym->GetMesh()->GetMesh();
	switch (mesh->Type())
	{
	case pm::MESH_POINTS:
		m_mesh = new PointsMesh(static_cast<const emesh::PointsMesh*>(sym->GetMesh()));
		break;
	case pm::MESH_TRIANGLES:
		m_mesh = new TrianglesMesh(static_cast<const pm::TrianglesMesh*>(mesh)->GetMeshData());
		break;
	case pm::MESH_SKIN2:
		m_mesh = new Skin2Mesh(static_cast<const pm::Skin2Mesh*>(mesh)->GetMeshData());
		break;
	default:
		throw ee::Exception("PackMesh::PackMesh unknown type %d", mesh->Type());
	}
}

PackMesh::~PackMesh()
{
	m_base->RemoveReference();
}

void PackMesh::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"mesh\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));
	if (!GetName().empty()) {
		lua::assign_with_end(gen, "export", "\"" + GetName() + "\"");
	}

	lua::connect(gen, 1, 
		lua::assign("base_id", m_base->GetID()));

	if (m_mesh) 
	{
		std::string type;
		switch (m_mesh->Type())
		{
		case simp::MESH_STRIP:
			type = "strip";
			break;
		case simp::MESH_POINTS:
			type = "network";
			break;
		case simp::MESH_TRIANGLES:
			type = "triangles";
			break;
		case simp::MESH_SKIN:
			type = "skeleton";
			break;
		case simp::MESH_SKIN2:
			type = "skeleton2";
			break;
		}

		lua::connect(gen, 1, 
			lua::assign("mesh_type", type));

		m_mesh->PackToLuaString(gen);
	}

	gen.detab();
	gen.line("},");
}

int PackMesh::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeMesh::Size();
	if (m_mesh) {
		sz += m_mesh->SizeOfUnpackFromBin();
	}
	return sz;
}

int PackMesh::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);		// id
	sz += sizeof(uint8_t);		// type
	sz += sizeof(uint32_t);		// base id
	if (m_mesh) {
		sz += sizeof(uint8_t);	// type
		sz += m_mesh->SizeOfPackToBin();
	}
	return sz;
}

void PackMesh::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_MESH;
	pack(type, ptr);

	assert(m_base);
	uint32_t base_id = m_base->GetID();
	pack(base_id, ptr);

	if (m_mesh) {
		uint8_t type = m_mesh->Type();
		pack(type, ptr);

		m_mesh->PackToBin(ptr);
	} else {
		uint8_t type = simp::MESH_UNKNOWN;
		pack(type, ptr);
	}
}

}