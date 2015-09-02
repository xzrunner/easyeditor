#ifndef _LIBCOCO_INODE_BUILDER_H_
#define _LIBCOCO_INODE_BUILDER_H_

namespace ebuilder { class CodeGenerator; }

namespace libcoco
{

class TexturePacker;

class INodeBuilder
{
public:
	virtual ~INodeBuilder() {}
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen, 
		const TexturePacker& tp) const = 0;
}; // INodeBuilder

}

#endif // _LIBCOCO_INODE_BUILDER_H_