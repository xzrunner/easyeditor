#include "P3dSprBuilder.h"
#include "IPackNode.h"
#include "PackNodeFactory.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>

#include <easyparticle3d.h>

#include <algorithm>

namespace erespacker
{

P3dSprBuilder::P3dSprBuilder(ExportNameSet& export_set)
	: m_export_set(export_set)
{
}

P3dSprBuilder::~P3dSprBuilder()
{
	for_each(m_nodes.begin(), m_nodes.end(), ee::DeletePointerFunctor<IPackNode>());	
}

void P3dSprBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	for (int i = 0, n = m_nodes.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(m_nodes[i], has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* P3dSprBuilder::Create(const eparticle3d::Sprite* spr)
{
	PackP3dSpr* node = new PackP3dSpr;
	node->p3d = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
	node->loop = spr->IsLoop();
	node->local = spr->IsLocalModeDraw();
	node->alone = spr->IsAlone();
	node->reuse = spr->IsReuse();
	m_nodes.push_back(node);
	return node;
}

void P3dSprBuilder::Create(const eparticle3d::Symbol* sym, const IPackNode* p3d)
{
	PackP3dSpr* node = new PackP3dSpr;

	node->p3d = p3d;
	node->loop = sym->IsLoop();
	node->local = sym->IsLocal();
	node->alone = false;
	node->reuse = false;
	m_nodes.push_back(node);

	m_export_set.LoadExport(sym, node);
}

}