#ifndef _LIBRESPACKER_INODE_BUILDER_H_
#define _LIBRESPACKER_INODE_BUILDER_H_

namespace d2d { class Visitor;}

namespace librespacker
{

class INodeBuilder
{
public:
	virtual ~INodeBuilder() {}
	virtual void Traverse(ee::Visitor& visitor) const = 0;
}; // INodeBuilder

}

#endif // _LIBRESPACKER_INODE_BUILDER_H_