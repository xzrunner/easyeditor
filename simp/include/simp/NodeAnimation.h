#ifndef _SIMP_NODE_ANIMATION_H_
#define _SIMP_NODE_ANIMATION_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;
class NodeTrans;

class NodeAnimation : private cu::Uncopyable
{
public:
	struct Actor
	{
		NodeTrans* trans;
		uint32_t spr;
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
	Layer layers[1];

public:
	NodeAnimation(Allocator& alloc, ImportStream& is);

	static int Size();
	static int ActorSize();
	static int FrameSize();
	static int LayerSize();

}; // NodeAnimation

}

#endif // _SIMP_NODE_ANIMATION_H_