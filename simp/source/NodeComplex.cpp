#include "NodeComplex.h"
#include "ImportStream.h"
#include "simp_define.h"

namespace simp
{

NodeComplex::NodeComplex(ImportStream& is)
{
}

int NodeComplex::Size()
{
	return sizeof(NodeComplex) + PTR_SIZE_DIFF - sizeof(Action);
}

}