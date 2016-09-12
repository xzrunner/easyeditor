#ifndef _SIMP_NODE_PICTURE_H_
#define _SIMP_NODE_PICTURE_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodePicture : private cu::Uncopyable
{
public:
	int texid;
	uint16_t texcoords[8];

public:
	NodePicture(ImportStream& is);

	static int Size();

}; // NodePicture

}

#endif // _SIMP_NODE_PICTURE_H_