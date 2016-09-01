#include "MaskBuilder.h"
#include "PackMask.h"
#include "PackNodeFactory.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>

#include <easymask.h>

#include <algorithm>

namespace erespacker
{

MaskBuilder::~MaskBuilder()
{
	for_each(m_nodes.begin(), m_nodes.end(), ee::DeletePointerFunctor<IPackNode>());
}

void MaskBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	for (int i = 0, n = m_nodes.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(m_nodes[i], has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* MaskBuilder::Create(const emask::Symbol* sym)
{
	PackMask* node = new PackMask;

	node->base = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(sym->GetBase()->GetSymbol()));
	node->mask = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(sym->GetMask()->GetSymbol()));

	m_nodes.push_back(node);

	return node;
}

}