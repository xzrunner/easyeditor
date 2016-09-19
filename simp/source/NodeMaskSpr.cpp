#include "NodeMaskSpr.h"
#include "ImportStream.h"

namespace simp
{

NodeMaskSpr::NodeMaskSpr(ImportStream& is)
{
	sym = is.UInt32();
}

int NodeMaskSpr::Size()
{
	return sizeof(NodeMaskSpr);
}

}