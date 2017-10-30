#ifndef _EASYRESPACKER_CLIPBOX_BUILDER_H_
#define _EASYRESPACKER_CLIPBOX_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace ecomplex { class Symbol; }

namespace erespacker
{

class IPackNode;
class PackClipbox;

class ClipboxBuilder : public INodeBuilder
{
public:
	ClipboxBuilder();
	virtual ~ClipboxBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const;

	const IPackNode* Create(const std::shared_ptr<const ecomplex::Symbol>& complex);

private:
	std::map<std::shared_ptr<const ecomplex::Symbol>, const PackClipbox*> m_map_clipbox;

}; // ClipboxBuilder

}

#endif // _EASYRESPACKER_CLIPBOX_BUILDER_H_
