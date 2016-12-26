#ifndef _SHADERLAB_GAUSSIAN_BLUR_HORI_PROG_H_
#define _SHADERLAB_GAUSSIAN_BLUR_HORI_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class GaussianBlurHoriProg : public FilterProgram
{
public:
	GaussianBlurHoriProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib);

	void SetTexWidth(float width);

private:
	int m_tex_width_id;
	float m_tex_width_val;

}; // GaussianBlurHoriProg

}

#endif // _SHADERLAB_GAUSSIAN_BLUR_HORI_PROG_H_