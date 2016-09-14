#include "NodeTrans.h"
#include "simp_define.h"

namespace simp
{

NodeTrans::NodeTrans()
	: name(NULL)
{
}

int NodeTrans::Size()
{
	return sizeof(NodeTrans) + PTR_SIZE_DIFF - sizeof(uint32_t);
}

}