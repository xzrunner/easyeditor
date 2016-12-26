#ifndef _SHADERLAB_GRAY_PROG_H_
#define _SHADERLAB_GRAY_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class GrayProg : public FilterProgram
{
public:
	GrayProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, 
		RenderBuffer* ib, parser::Node* pre = NULL);

}; // GrayProg

}

#endif // _SHADERLAB_GRAY_PROG_H_