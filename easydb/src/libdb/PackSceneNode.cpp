#include "PackSceneNode.h"
#include "check_params.h"

#include <bsn/NodeFactory.h>
#include <bsn/NodeSym.h>
#include <memmgr/LinearAllocator.h>

#include <fstream>

namespace edb
{

std::string PackSceneNode::Command() const
{
	return "pack-scene-node";
}

std::string PackSceneNode::Description() const
{
	return "pack scene node";
}

std::string PackSceneNode::Usage() const
{
	std::string usage = Command() + " [src file] [dst file]";
	return usage;
}

int PackSceneNode::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_file(argv[2])) return -1;

	Pack(argv[2], argv[3]);

	return 0;
}

void PackSceneNode::Pack(const std::string& src_file, const std::string& dst_file)
{
	mm::LinearAllocator alloc;
	bsn::NodeSym* sym = bsn::NodeFactory::CreateNodeSym(alloc, src_file);

	uint8_t* data = nullptr;
	size_t len = 0;
	sym->StoreToBin(&data, len);

	std::ofstream fout(dst_file, std::ios::binary);
	fout.write(reinterpret_cast<const char*>(data), len);
	fout.close();
	delete data;
}

}