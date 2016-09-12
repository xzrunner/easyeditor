#include "NodeComplexSpr.h"
#include "ImportStream.h"

namespace simp
{

NodeComplexSpr::NodeComplexSpr(ImportStream& is)
{
	sym = is.UInt32();
	action = is.UInt16();
}

int NodeComplexSpr::Size()
{
	return sizeof(NodeComplexSpr);
}

}