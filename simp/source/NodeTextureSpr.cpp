#include "NodeTextureSpr.h"
#include "ImportStream.h"

namespace simp
{

NodeTextureSpr::NodeTextureSpr(ImportStream& is)
{
	sym = is.UInt32();
}

int NodeTextureSpr::Size()
{
	return sizeof(NodeTextureSpr);
}

}