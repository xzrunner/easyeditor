#include "MeshSprBuilder.h"
#include "PackMesh.h"
#include "PackNodeFactory.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>

#include <easymesh.h>

#include <algorithm>

namespace erespacker
{

MeshSprBuilder::MeshSprBuilder(ExportNameSet& export_set)
	: m_export_set(export_set)
{
}

MeshSprBuilder::~MeshSprBuilder()
{
	for_each(m_nodes.begin(), m_nodes.end(), ee::DeletePointerFunctor<IPackNode>());
}

void MeshSprBuilder::Traverse(ee::Visitor& visitor) const
{
	for (int i = 0, n = m_nodes.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(m_nodes[i], has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* MeshSprBuilder::Create(const emesh::Symbol* sym)
{
	PackMesh* node = new PackMesh;

	node->base_sym = PackNodeFactory::Instance()->Create(sym->GetMesh()->GetBaseSymbol());

	const std::vector<emesh::Triangle*>& tris = sym->GetMesh()->GetTriangles();
	node->triangles.reserve(tris.size() * 3);
	for (int i = 0, n = tris.size(); i < n; ++i) {
		emesh::Triangle* tri = tris[i];
		for (int j = 0; j < 3; ++j) {
			node->triangles.push_back(tri->nodes[j]->xy);
		}
	}

	return node;
}

}