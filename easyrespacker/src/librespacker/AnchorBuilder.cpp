#include "AnchorBuilder.h"
#include "PackAnchor.h"

#include <ee/std_functor.h>

#include <algorithm>

namespace erespacker
{

AnchorBuilder::AnchorBuilder()
{
}

AnchorBuilder::~AnchorBuilder()
{
	for_each(m_anchors.begin(), m_anchors.end(), ee::DeletePointerFunctor<const PackAnchor>());
}

void AnchorBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
}

const IPackNode* AnchorBuilder::Create(const ee::Sprite* spr)
{
	return new PackAnchor();
}

}