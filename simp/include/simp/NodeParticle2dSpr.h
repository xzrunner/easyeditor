#ifndef _SIMP_NODE_PARTICLE2D_SPR_H_
#define _SIMP_NODE_PARTICLE2D_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeParticle2dSpr : private cu::Uncopyable
{
public:
	uint32_t sym;
	uint8_t loop;
	uint8_t local;

public:
	NodeParticle2dSpr(ImportStream& is);

	static int Size();

}; // NodeParticle2dSpr

}

#endif // _SIMP_NODE_PARTICLE2D_SPR_H_