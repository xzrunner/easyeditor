#include "MeshSprBuilder.h"
#include "PackMeshSpr.h"
#include "PackNodeFactory.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>

#include <easymesh.h>

#include <sprite2/MeshTriangle.h>

#include <algorithm>

namespace erespacker
{

MeshSprBuilder::~MeshSprBuilder()
{
	for_each(m_nodes.begin(), m_nodes.end(), ee::DeletePointerFunctor<IPackNode>());
}

void MeshSprBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	for (int i = 0, n = m_nodes.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(m_nodes[i], has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* MeshSprBuilder::Create(const emesh::Sprite* spr)
{
	PackMeshSpr* node = new PackMeshSpr;

	node->mesh = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));

	node->base = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(spr->GetBaseSym()));
	
	//////////////////////////////////////////////////////////////////////////

// 	const std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>& map = spr->GetMeshTrans().GetMap();
// 	node->trans_pairs.reserve(map.size() * 2);
// 	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr = map.begin();
// 	for ( ; itr != map.end(); ++itr) {
// 		node->trans_pairs.push_back(itr->first);
// 		node->trans_pairs.push_back(itr->second);
// 	}

	//////////////////////////////////////////////////////////////////////////

	const s2::Mesh* mesh = dynamic_cast<const s2::MeshSymbol*>(spr->GetSymbol())->GetMesh();
	const std::vector<s2::MeshTriangle*>& tris = mesh->GetTriangles();
	const std::vector<std::pair<int, sm::vec2> >& trans = spr->GetMeshTrans().GetTrans();
	for (int i = 0, n = trans.size(); i < n; ++i) 
	{
		int idx = trans[i].first;
		int tri = idx / 3;
		int ver = idx - tri * 3;

		const sm::vec2& from = tris[tri]->nodes[ver]->ori_xy;
		sm::vec2 to = from + trans[i].second;
 		node->trans_pairs.push_back(from);
 		node->trans_pairs.push_back(to);
	}

	//////////////////////////////////////////////////////////////////////////

	m_nodes.push_back(node);

	return node;
}

}