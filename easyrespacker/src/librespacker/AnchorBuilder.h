#ifndef _EASYRESPACKER_ANCHOR_BUILDER_H_
#define _EASYRESPACKER_ANCHOR_BUILDER_H_

#include "INodeBuilder.h"

#include <ee/Sprite.h>

#include <vector>

namespace erespacker
{

class IPackNode;
class PackAnchor;

class AnchorBuilder : public INodeBuilder
{
public:
	AnchorBuilder();
	virtual ~AnchorBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const;

	const IPackNode* Create(const ee::SprConstPtr& spr);

private:
	std::vector<const PackAnchor*> m_anchors;

}; // AnchorBuilder

}

#endif // _EASYRESPACKER_ANCHOR_BUILDER_H_
