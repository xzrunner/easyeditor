#ifndef _LIBCOCO_INODE_PACKER_H_
#define _LIBCOCO_INODE_PACKER_H_

namespace ebuilder { class CodeGenerator; }

namespace libcoco
{

class TexturePacker;

class INodePacker
{
public:
	virtual ~INodePacker() {}
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen, 
		const TexturePacker& tp) const = 0;
}; // INodePacker

}

#endif // _LIBCOCO_INODE_PACKER_H_