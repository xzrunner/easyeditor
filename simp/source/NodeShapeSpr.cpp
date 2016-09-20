#include "NodeShapeSpr.h"
#include "ImportStream.h"

namespace simp
{

NodeShapeSpr::NodeShapeSpr(ImportStream& is)
{
	sym = is.UInt32();
}

int NodeShapeSpr::Size()
{
	return sizeof(NodeShapeSpr);
}

}