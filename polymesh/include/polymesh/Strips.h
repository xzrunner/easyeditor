#ifndef _POLYMESH_STRIPS_H_
#define _POLYMESH_STRIPS_H_

#include "Vertex.h"

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace pm
{

class Strips : private cu::Uncopyable
{
public:
	uint16_t  num;
	uint16_t  padding;	
	uint16_t* strip_num;
	Vertex    vertices[1];
	
}; // Strips

}

#endif // _POLYMESH_STRIPS_H_