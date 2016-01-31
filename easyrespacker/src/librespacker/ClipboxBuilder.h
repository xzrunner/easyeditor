#ifndef _LIBRESPACKER_CLIPBOX_BUILDER_H_
#define _LIBRESPACKER_CLIPBOX_BUILDER_H_

#include "INodeBuilder.h"


#include <easycomplex.h>

namespace librespacker
{

class IPackNode;
class PackClipbox;

class ClipboxBuilder : public INodeBuilder
{
public:
	ClipboxBuilder();
	virtual ~ClipboxBuilder();

	virtual void Traverse(ee::Visitor& visitor) const;

	const IPackNode* Create(const ecomplex::Symbol* complex);

private:
	std::map<const ecomplex::Symbol*, const PackClipbox*> m_map_clipbox;

}; // ClipboxBuilder

}

#endif // _LIBRESPACKER_CLIPBOX_BUILDER_H_
