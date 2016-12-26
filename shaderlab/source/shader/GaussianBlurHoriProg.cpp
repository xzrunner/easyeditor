#include "GaussianBlurHoriProg.h"
#include "RenderShader.h"
#include "GaussianBlurHori.h"

#include <unirender/typedef.h>

namespace sl
{

GaussianBlurHoriProg::GaussianBlurHoriProg(ur::RenderContext* rc, int max_vertex, 
										   const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
	, m_tex_width_val(0)
{
	Init(va_list, ib, new parser::GaussianBlurHori());

	m_tex_width_id = m_shader->AddUniform("u_tex_width", ur::UNIFORM_FLOAT1);
}

void GaussianBlurHoriProg::SetTexWidth(float width)
{
	if (width != m_tex_width_val) {
		m_tex_width_val = width;
		m_shader->SetUniform(m_tex_width_id, ur::UNIFORM_FLOAT1, &width);
	}
}

}