#include "PackMesh.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "PackCoords.h"

#include "NetworkMesh.h"
#include "TrianglesMesh.h"
#include "Skeleton2Mesh.h"

#include <easymesh.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <sprite2/NetworkMesh.h>
#include <sprite2/TrianglesMesh.h>
#include <sprite2/Skeleton2Mesh.h>
#include <simp/NodeMesh.h>
#include <simp/simp_types.h>
#include <simp/MeshType.h>

namespace esprpacker
{

PackMesh::PackMesh(const emesh::Symbol* sym)
	: m_mesh(NULL)
{
	m_base = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(
		sym->GetMesh()->GetBaseSymbol()));

	const s2::Mesh* mesh = sym->GetMesh();
	switch (mesh->Type())
	{
	case s2::MESH_NETWORK:
		m_mesh = new NetworkMesh(VI_DOWNCASTING<const s2::NetworkMesh*>(mesh));		
		break;
	case s2::MESH_TRIANGLES:
		m_mesh = new TrianglesMesh(VI_DOWNCASTING<const s2::TrianglesMesh*>(mesh));
		break;
	case s2::MESH_SKELETON2:
		m_mesh = new Skeleton2Mesh(VI_DOWNCASTING<const s2::Skeleton2Mesh*>(mesh));
		break;
	default:
		throw ee::Exception("PackMesh::PackMesh unknown type %d", mesh->Type());
	}
}

PackMesh::~PackMesh()
{
	m_base->RemoveReference();
	delete m_mesh;
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
		case simp::MESH_NETWORK:
			type = "network";
			break;
		case simp::MESH_TRIANGLES:
			type = "triangles";
			break;
		case simp::MESH_SKELETON:
			type = "skeleton";
			break;
		case simp::MESH_SKELETON2:
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