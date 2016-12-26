#ifndef _SHADERLAB_RELIEF_PROG_H_
#define _SHADERLAB_RELIEF_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class ReliefProg : public FilterProgram
{
public:
	ReliefProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib);

}; // ReliefProg

}

#endif // _SHADERLAB_RELIEF_PROG_H_