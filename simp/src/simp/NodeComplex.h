#ifndef _SPRITE_IMPORT_NODE_COMPLEX_H_
#define _SPRITE_IMPORT_NODE_COMPLEX_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

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
	uint16_t sprs_n;

	uint16_t actions_n;

	int32_t scissor[4];

	Action actions[1];

public:
	NodeComplex(ImportStream& is);

	static int Size();

}; // NodeComplex

}

#endif // _SPRITE_IMPORT_NODE_COMPLEX_H_