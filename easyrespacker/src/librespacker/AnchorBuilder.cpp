#include "AnchorBuilder.h"
#include "PackAnchor.h"

namespace librespacker
{

AnchorBuilder::AnchorBuilder()
{
}

AnchorBuilder::~AnchorBuilder()
{
	for_each(m_anchors.begin(), m_anchors.end(), DeletePointerFunctor<const PackAnchor>());
}

void AnchorBuilder::Traverse(d2d::IVisitor& visitor) const
{
}

const IPackNode* AnchorBuilder::Create(const d2d::ISprite* spr)
{
	return new PackAnchor();
}

}