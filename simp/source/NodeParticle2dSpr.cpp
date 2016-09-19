#include "NodeParticle2dSpr.h"
#include "ImportStream.h"

namespace simp
{

NodeParticle2dSpr::NodeParticle2dSpr(ImportStream& is)
{
	sym		= is.UInt32();
	loop	= is.UInt8();
	local	= is.UInt8();
}

int NodeParticle2dSpr::Size()
{
	return sizeof(NodeParticle2dSpr);
}

}