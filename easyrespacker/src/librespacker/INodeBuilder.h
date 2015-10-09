#ifndef _LIBRESPACKER_INODE_BUILDER_H_
#define _LIBRESPACKER_INODE_BUILDER_H_

namespace d2d { class IVisitor;}

namespace librespacker
{

class INodeBuilder
{
public:
	virtual ~INodeBuilder() {}
	virtual void Traverse(d2d::IVisitor& visitor) const = 0;
}; // INodeBuilder

}

#endif // _LIBRESPACKER_INODE_BUILDER_H_