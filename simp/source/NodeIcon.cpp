#include "NodeIcon.h"
#include "ImportStream.h"

namespace simp
{

NodeIcon::NodeIcon(ImportStream& is)
{
	type = is.UInt32();
}

int NodeIcon::Size()
{
	return sizeof(NodeIcon);
}

}