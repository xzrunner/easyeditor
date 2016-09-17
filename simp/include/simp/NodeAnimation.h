#ifndef _SIMP_NODE_ANIMATION_H_
#define _SIMP_NODE_ANIMATION_H_

#include <CU_Uncopyable.h>

namespace simp
{

class NodeAnimation : private cu::Uncopyable
{
public:
	struct Actor
	{
		uint32_t spr;
		const char* name;
	};

	struct Frame
	{
		Actor** actors;
		uint16_t n;
		uint16_t index;
		uint8_t tween;
	};

	struct Layer
	{
		Frame** frames;
		int n;
	};

	uint16_t n;
	Layer layer[1];

public:
	NodeAnimation(Allocator& alloc, ImportStream& is);

	static int Size();
	static int ActorSize();
	static int FrameSize();
	static int LayerSize();

}; // NodeAnimation

}

#endif // _SIMP_NODE_ANIMATION_H_