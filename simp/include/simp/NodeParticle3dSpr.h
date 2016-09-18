#ifndef _SIMP_NODE_PARTICLE3D_SPR_H_
#define _SIMP_NODE_PARTICLE3D_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeParticle3dSpr : private cu::Uncopyable
{
public:
	uint32_t sym;
	uint8_t loop;
	uint8_t local;
	uint8_t alone;
	uint8_t reuse;

public:
	NodeParticle3dSpr(ImportStream& is);

	static int Size();

}; // NodeParticle3dSpr

}

#endif // _SIMP_NODE_PARTICLE3D_SPR_H_