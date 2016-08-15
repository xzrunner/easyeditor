#include "MeshSprBuilder.h"
#include "PackMeshSpr.h"
#include "PackNodeFactory.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>

#include <easymesh.h>

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

	node->mesh = PackNodeFactory::Instance()->Create(spr->GetSymbol());

	node->base = PackNodeFactory::Instance()->Create(spr->GetBaseSym());
	
	const std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>& map = spr->GetMeshTrans().GetMap();
	node->trans_pairs.reserve(map.size() * 2);
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr = map.begin();
	for ( ; itr != map.end(); ++itr) {
		node->trans_pairs.push_back(itr->first);
		node->trans_pairs.push_back(itr->second);
	}

	m_nodes.push_back(node);

	return node;
}

}