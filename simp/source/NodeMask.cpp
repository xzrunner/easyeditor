#include "NodeMask.h"
#include "ImportStream.h"

namespace simp
{

NodeMask::NodeMask(ImportStream& is)
{
	base_id = is.UInt32();
	mask_id = is.UInt32();
}

int NodeMask::Size()
{
	return sizeof(NodeMask);
}

}