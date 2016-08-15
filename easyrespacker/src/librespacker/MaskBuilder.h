#ifndef _EASYRESPACKER_MASK_BUILDER_H_
#define _EASYRESPACKER_MASK_BUILDER_H_

#include "INodeBuilder.h"
#include "PackMask.h"

namespace emask { class Symbol; }

namespace erespacker
{

class ExportNameSet;

class MaskBuilder : public INodeBuilder
{
public:
	MaskBuilder() {}
	virtual ~MaskBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const;

	const IPackNode* Create(const emask::Symbol* sym);

private:
	std::vector<IPackNode*> m_nodes;

}; // MaskBuilder

}

#endif // _EASYRESPACKER_MASK_BUILDER_H_