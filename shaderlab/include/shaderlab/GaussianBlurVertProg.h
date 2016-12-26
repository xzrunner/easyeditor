#ifndef _SHADERLAB_GAUSSIAN_BLUR_VERT_PROG_H_
#define _SHADERLAB_GAUSSIAN_BLUR_VERT_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class GaussianBlurVertProg : public FilterProgram
{
public:
	GaussianBlurVertProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib);

	void SetTexHeight(float height);

private:
	int m_tex_height_id;
	float m_tex_height_val;

}; // GaussianBlurVertProg

}

#endif // _SHADERLAB_GAUSSIAN_BLUR_VERT_PROG_H_