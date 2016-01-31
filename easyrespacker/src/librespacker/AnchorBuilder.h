#ifndef _LIBRESPACKER_ANCHOR_BUILDER_H_
#define _LIBRESPACKER_ANCHOR_BUILDER_H_

#include "INodeBuilder.h"



namespace librespacker
{

class IPackNode;
class PackAnchor;

class AnchorBuilder : public INodeBuilder
{
public:
	AnchorBuilder();
	virtual ~AnchorBuilder();

	virtual void Traverse(ee::Visitor& visitor) const;

	const IPackNode* Create(const ee::Sprite* spr);

private:
	std::vector<const PackAnchor*> m_anchors;

}; // AnchorBuilder

}

#endif // _LIBRESPACKER_ANCHOR_BUILDER_H_
