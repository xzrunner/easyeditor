#include "PackMeshSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"
#include "PackCoords.h"

#include <easymesh.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeMeshSpr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackMeshSpr::PackMeshSpr(const emesh::Sprite* spr)
{
	m_mesh = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
	m_base = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetBaseSym()));

	const std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>& map = spr->GetMeshTrans().GetMap();
	m_trans_pairs.reserve(map.size() * 2);
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr = map.begin();
	for ( ; itr != map.end(); ++itr) {
		m_trans_pairs.push_back(itr->first);
		m_trans_pairs.push_back(itr->second);
	}
}

PackMeshSpr::~PackMeshSpr()
{
	m_mesh->RemoveReference();
	m_base->RemoveReference();
}

void PackMeshSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::assign_with_end(gen, "type", "\"mesh_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::connect(gen, 1, 
		lua::assign("mesh_id", m_mesh->GetID()));
	lua::connect(gen, 1, 
		lua::assign("base_id", m_base->GetID()));

	PackCoords::PackToLua(gen, m_trans_pairs, "trans_pairs");

	gen.detab();
	gen.line("},");
}

int PackMeshSpr::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeMeshSpr::Size();
	sz += PackCoords::SizeOfUnpackFromBin(m_trans_pairs);
	return sz;
}

int PackMeshSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);									// id
	sz += sizeof(uint8_t);									// type
	sz += sizeof(uint32_t);									// mesh id
	sz += sizeof(uint32_t);									// base id
	sz += PackCoords::SizeOfPackToBin(m_trans_pairs);		// trans
	return sz;
}

void PackMeshSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_MESH_SPR;
	pack(type, ptr);

	assert(m_mesh);
	uint32_t mesh_id = m_mesh->GetID();
	pack(mesh_id, ptr);	

	assert(m_base);
	uint32_t base_id = m_base->GetID();
	pack(base_id, ptr);

	PackCoords::PackToBin(m_trans_pairs, ptr);
}

bool PackMeshSpr::Equal(const emesh::Sprite* spr) const
{
	const PackNode* base = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetBaseSym()));
	if (m_base != base) {
		return false;
	}
	const std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>& map = spr->GetMeshTrans().GetMap();
	if (m_trans_pairs.size() != map.size() * 2) {
		return false;
	}
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr = map.begin();
	int idx = 0;
	for ( ; itr != map.end(); ++itr) {
		if (itr->first != m_trans_pairs[idx++] ||
			itr->second != m_trans_pairs[idx++]) {
			return false;
		}
	}
	return true;
}

}