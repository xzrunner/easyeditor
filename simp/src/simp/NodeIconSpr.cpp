#include "NodeIconSpr.h"
#include "ImportStream.h"

namespace simp
{

NodeIconSpr::NodeIconSpr(ImportStream& is)
{
	sym = is.UInt32();
	process = is.UInt16();
}

int NodeIconSpr::Size()
{
	return sizeof(NodeIconSpr);
}

}