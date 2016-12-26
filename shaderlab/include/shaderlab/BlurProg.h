#ifndef _SHADERLAB_BLUR_PROG_H_
#define _SHADERLAB_BLUR_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class BlurProg : public FilterProgram
{
public:
	BlurProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib);

	void SetRadius(float r);

private:
	int m_radius;

}; // BlurProg

}

#endif // _SHADERLAB_BLUR_PROG_H_