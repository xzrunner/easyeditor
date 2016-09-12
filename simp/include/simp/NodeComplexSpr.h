#ifndef _SIMP_NODE_COMPLEX_SPR_H_
#define _SIMP_NODE_COMPLEX_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeComplexSpr : private cu::Uncopyable
{
public:
	uint32_t sym;
	uint32_t action;	// uint16_t enough
	
public:
	NodeComplexSpr(ImportStream& is);

	static int Size();		

}; // NodeComplexSpr

}

#endif // _SIMP_NODE_COMPLEX_SPR_H_