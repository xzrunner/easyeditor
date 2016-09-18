#ifndef _SIMP_NODE_COMPLEX_H_
#define _SIMP_NODE_COMPLEX_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;
class NodeTrans;

class NodeComplex : private cu::Uncopyable
{
public:
	struct Action
	{
		const char* name;
		uint16_t* idx;
		uint16_t n;
	};

	uint32_t* sprs;
	NodeTrans** trans;
	uint16_t sprs_n;

	uint16_t actions_n;

	int16_t scissor[4];

	Action actions[1];

public:
	NodeComplex(Allocator& alloc, ImportStream& is);

	static int Size();
	static int ActionSize();

}; // NodeComplex

}

#endif // _SIMP_NODE_COMPLEX_H_