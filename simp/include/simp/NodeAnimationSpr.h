#ifndef _SIMP_NODE_ANIMATION_SPR_H_
#define _SIMP_NODE_ANIMATION_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeAnimationSpr : private cu::Uncopyable
{
public:
	uint32_t sym;
	uint32_t interval;
	uint16_t fps;
	uint8_t  loop;

public:
	NodeAnimationSpr(ImportStream& is);

	static int Size();

}; // NodeAnimationSpr

}

#endif // _SIMP_NODE_ANIMATION_SPR_H_