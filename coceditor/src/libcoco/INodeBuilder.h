#ifndef _LIBCOCO_INODE_BUILDER_H_
#define _LIBCOCO_INODE_BUILDER_H_

namespace d2d { class IVisitor;}

namespace libcoco
{

class TexturePacker;

class INodeBuilder
{
public:
	virtual ~INodeBuilder() {}
	virtual void Traverse(d2d::IVisitor& visitor) const = 0;
}; // INodeBuilder

}

#endif // _LIBCOCO_INODE_BUILDER_H_