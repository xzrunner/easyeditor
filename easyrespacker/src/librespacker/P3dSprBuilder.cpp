#include "P3dSprBuilder.h"
#include "IPackNode.h"
#include "PackNodeFactory.h"

#include <easyparticle3d.h>

namespace librespacker
{

P3dSprBuilder::P3dSprBuilder()
{
}

P3dSprBuilder::~P3dSprBuilder()
{
	for_each(m_nodes.begin(), m_nodes.end(), DeletePointerFunctor<IPackNode>());	
}

void P3dSprBuilder::Traverse(d2d::IVisitor& visitor) const
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
	node->p3d = PackNodeFactory::Instance()->Create(&spr->GetSymbol());
	node->loop = spr->IsLoop();
	node->local = spr->IsLocalModeDraw();
	node->alone = spr->IsAlone();
	node->reuse = spr->IsReuse();
	m_nodes.push_back(node);
	return node;
}

}