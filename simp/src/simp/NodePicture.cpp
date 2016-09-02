#include "NodePicture.h"
#include "ImportStream.h"

namespace simp
{

NodePicture::NodePicture(ImportStream& is)
{
	texid = is.UInt8();
	for (int i = 0; i < 8; ++i) {
		texcoords[i] = is.UInt16();
	}
}

int NodePicture::Size()
{
	return sizeof(NodePicture);
}

}