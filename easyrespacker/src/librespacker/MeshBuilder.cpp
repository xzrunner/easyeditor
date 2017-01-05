#include "MeshBuilder.h"
#include "PackMesh.h"
#include "PackNodeFactory.h"
#include "PackUI.h"
#include "PackTag.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>

#include <easymesh.h>

#include <sprite2/MeshTriangle.h>

#include <algorithm>

namespace erespacker
{

MeshBuilder::~MeshBuilder()
{
	for_each(m_nodes.begin(), m_nodes.end(), ee::DeletePointerFunctor<IPackNode>());
}

void MeshBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	for (int i = 0, n = m_nodes.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(m_nodes[i], has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* MeshBuilder::Create(const emesh::Symbol* sym)
{
	PackMesh* node = new PackMesh;

	const s2::Symbol* base = sym->GetMesh()->GetBaseSymbol();
	node->base = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(base));

	const std::vector<s2::MeshTriangle*>& tris = sym->GetMesh()->GetTriangles();
	int sz = tris.size() * 3;
	node->tri_vertices.reserve(sz);
	node->tri_vertices.reserve(sz);
	for (int i = 0, n = tris.size(); i < n; ++i) {
		s2::MeshTriangle* tri = tris[i];
		for (int j = 0; j < 3; ++j) {
			node->tri_texcoords.push_back(tri->nodes[j]->uv);
			node->tri_vertices.push_back(tri->nodes[j]->ori_xy);
		}
	}

	m_nodes.push_back(node);

	PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());
	PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());	

	return node;
}

}