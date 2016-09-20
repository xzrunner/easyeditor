#include "PackMesh.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "PackVertices.h"

#include <easymesh.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <sprite2/MeshTriangle.h>
#include <sprite2/NetworkMesh.h>
#include <sprite2/NetworkShape.h>
#include <simp/NodeMesh.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackMesh::PackMesh(const emesh::Symbol* sym)
{
	m_base = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(
		sym->GetMesh()->GetBaseSymbol()));

	const s2::NetworkMesh* nw_mesh = VI_DOWNCASTING<const s2::NetworkMesh*>(sym->GetMesh());
	m_outer_line = nw_mesh->GetShape()->GetVertices();
	m_inner_line = nw_mesh->GetShape()->GetInnerVertices();
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
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));
	if (!m_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + m_name + "\"");
	}

	lua::connect(gen, 1, 
		lua::assign("base_id", m_base->GetID()));

	PackVertices::PackToLua(gen, m_outer_line, "outer");
	PackVertices::PackToLua(gen, m_inner_line, "inner");

	gen.detab();
	gen.line("},");
}

int PackMesh::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeMesh::Size();
	sz += PackVertices::SizeOfUnpackFromBin(m_outer_line);
	sz += PackVertices::SizeOfUnpackFromBin(m_inner_line);
	return sz;
}

int PackMesh::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);								// id
	sz += sizeof(uint8_t);								// type
	sz += sizeof(uint32_t);								// base id
	sz += PackVertices::SizeOfPackToBin(m_outer_line);	// outer
	sz += PackVertices::SizeOfPackToBin(m_inner_line);	// inner
	return sz;
}

void PackMesh::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = simp::TYPE_MESH;
	pack(type, ptr);

	assert(m_base);
	uint32_t base_id = m_base->GetID();
	pack(base_id, ptr);

	PackVertices::PackToBin(m_outer_line, ptr);
	PackVertices::PackToBin(m_inner_line, ptr);
}

}