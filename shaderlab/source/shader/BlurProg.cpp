#include "BlurProg.h"
#include "RenderShader.h"
#include "Blur.h"

#include <unirender/typedef.h>

namespace sl
{

BlurProg::BlurProg(ur::RenderContext* rc, int max_vertex, 
				   const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Blur());
	m_radius = m_shader->AddUniform("u_radius", ur::UNIFORM_FLOAT1);
}

void BlurProg::SetRadius(float r)
{
	m_shader->SetUniform(m_radius, ur::UNIFORM_FLOAT1, &r);
}

}