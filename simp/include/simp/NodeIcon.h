#ifndef _SPRITE_IMPORT_NODE_ICON_H_
#define _SPRITE_IMPORT_NODE_ICON_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeIcon : private cu::Uncopyable
{
public:
	uint32_t type;

public:
	NodeIcon(ImportStream& is);

	static int Size();

}; // NodeIcon

}

#endif // _SPRITE_IMPORT_NODE_ICON_H_