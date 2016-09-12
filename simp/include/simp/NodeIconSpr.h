#ifndef _SIMP_NODE_ICON_SPR_H_
#define _SIMP_NODE_ICON_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeIconSpr : private cu::Uncopyable
{
public:
	uint32_t sym;
	uint16_t process;
	uint16_t dummy;

public:
	NodeIconSpr(ImportStream& is);

	static int Size();

}; // NodeIconSpr

}

#endif // _SIMP_NODE_ICON_SPR_H_