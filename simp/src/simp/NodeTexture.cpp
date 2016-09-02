#include "NodeTexture.h"
#include "ImportStream.h"

namespace simp
{

NodeTexture::NodeTexture(ImportStream& is)
{
	type = is.UInt8();
}

int NodeTexture::Size()
{
	return sizeof(NodeTexture);
}

}