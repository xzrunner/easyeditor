#include "ReliefProg.h"
#include "RenderShader.h"
#include "Relief.h"

namespace sl
{

ReliefProg::ReliefProg(ur::RenderContext* rc, int max_vertex, 
					   const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Relief());
}

}