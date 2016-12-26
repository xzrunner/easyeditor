#include "OutlineProg.h"
#include "RenderShader.h"
#include "Outline.h"

namespace sl
{

OutlineProg::OutlineProg(ur::RenderContext* rc, int max_vertex, 
						 const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Outline());
}

}