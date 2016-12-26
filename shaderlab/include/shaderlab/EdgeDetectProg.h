#ifndef _SHADERLAB_EDGE_DETECT_PROG_H_
#define _SHADERLAB_EDGE_DETECT_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class EdgeDetectProg : public FilterProgram
{
public:
	EdgeDetectProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib);

	void SetBlend(float blend);

private:
	int m_blend;

}; // EdgeDetectProg

}

#endif // _SHADERLAB_EDGE_DETECT_PROG_H_