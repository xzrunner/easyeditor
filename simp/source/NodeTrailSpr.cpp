#include "NodeTrailSpr.h"
#include "ImportStream.h"

namespace simp
{

NodeTrailSpr::NodeTrailSpr(ImportStream& is)
{
	sym = is.UInt32();
}

int NodeTrailSpr::Size()
{
	return sizeof(NodeTrailSpr);
}

}