#include "NodeAnimation.h"

namespace simp
{

NodeAnimation::NodeAnimation(Allocator& alloc, ImportStream& is)
{
	n = 
}

int NodeAnimation::Size()
{
	sizeof(NodeAnimation) - sizeof(Layer);
}

int NodeAnimation::ActorSize()
{
	return sizeof(Actor) + PTR_SIZE_DIFF;
}

int NodeAnimation::FrameSize()
{
	return sizeof(Frame) + PTR_SIZE_DIFF;
}

int NodeAnimation::LayerSize()
{
	return sizeof(Layer) + PTR_SIZE_DIFF;		
}

}