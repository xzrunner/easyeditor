#include "PackMeshSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"
#include "PackCoords.h"

#include <easymesh.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <bimp/bimp_typedef.h>
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

	//////////////////////////////////////////////////////////////////////////

	//const std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>& map = spr->GetMeshTrans().GetMap();
	//m_trans_pairs.reserve(map.size() * 2);
	//std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr = map.begin();
	//for ( ; itr != map.end(); ++itr) {
	//	m_trans_pairs.push_back(itr->first);
	//	m_trans_pairs.push_back(itr->second);
	//}

	//////////////////////////////////////////////////////////////////////////

// 	const s2::Mesh* mesh = dynamic_cast<const s2::MeshSymbol*>(spr->GetSymbol())->GetMesh();
// 	const std::vector<s2::MeshTriangle*>& tris = mesh->GetTriangles();
// 	const std::vector<std::pair<int, sm::vec2> >& trans = spr->GetMeshTrans().GetTrans();
// 	for (int i = 0, n = trans.size(); i < n; ++i) 
// 	{
// 		int idx = trans[i].first;
// 		int tri = idx / 3;
// 		int ver = idx - tri * 3;
// 
// 		const sm::vec2& from = tris[tri]->nodes[ver]->ori_xy;
// 		sm::vec2 to = from + trans[i].second;
// 		m_trans_pairs.push_back(from);
// 		m_trans_pairs.push_back(to);
// 	}

	//////////////////////////////////////////////////////////////////////////

	m_transform = spr->GetMeshTrans().GetTrans();
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

	
	lua::assign_with_end(gen, "trans_num", m_transform.size());
	std::stringstream ss;
	ss << "trans = {";
	for (int i = 0, n = m_transform.size(); i < n; ++i) 
	{
		int idx = m_transform[i].first;
		int16_t x = float2int(m_transform[i].second.x, 16),
			    y = float2int(m_transform[i].second.y, 16);
		ss << idx << ", " << x << ", " << y << ", ";
	}
	ss << "}";
	gen.line(ss.str());

	gen.detab();
	gen.line("},");
}

int PackMeshSpr::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeMeshSpr::Size();
	sz += ALIGN_4BYTE(sizeof(uint16_t) * 3 * m_transform.size());
	return sz;
}

int PackMeshSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);									// id
	sz += sizeof(uint8_t);									// type
	sz += sizeof(uint32_t);									// mesh id
	sz += sizeof(uint32_t);									// base id
	// trans
	sz += sizeof(uint16_t);
	sz += sizeof(uint16_t) * 3 * m_transform.size();
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

	uint16_t num = m_transform.size();
	pack(num, ptr);
	for (int i = 0, n = m_transform.size(); i < n; ++i) 
	{
		uint16_t idx = m_transform[i].first;
		pack(idx, ptr);

		const sm::vec2& pos = m_transform[i].second;
		int16_t x = float2int(pos.x, 16),
			    y = float2int(pos.y, 16);
		pack(x, ptr);
		pack(y, ptr);
	}
}

bool PackMeshSpr::Equal(const emesh::Sprite* spr) const
{
	const PackNode* base = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetBaseSym()));
	if (m_base != base) {
		return false;
	}

	const std::vector<std::pair<int, sm::vec2> >& trans 
		= spr->GetMeshTrans().GetTrans();
	if (m_transform.size() != trans.size()) {
		return false;
	}
	for (int i = 0, n = trans.size(); i < n; ++i) {
		const std::pair<int, sm::vec2>& t0 = m_transform[i];
		const std::pair<int, sm::vec2>& t1 = trans[i];
		if (t0.first != t1.first ||
			t0.second != t1.second) {
			return false;
		}
	}

	return true;
}

}