#include "ColGradingProg.h"
#include "RenderShader.h"
#include "ColorGrading.h"

#include <unirender/RenderContext.h>

namespace sl
{

ColGradingProg::ColGradingProg(ur::RenderContext* rc, int max_vertex, 
						   const std::vector<ur::VertexAttrib>& va_list, 
						   RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
	, m_lut_tex(0)
{
	Init(va_list, ib, new parser::ColorGrading());

	int tex0 = m_shader->AddUniform("u_texture0", ur::UNIFORM_INT1);
	if (tex0 >= 0) {
		float sample = 0;
		m_shader->SetUniform(tex0, ur::UNIFORM_INT1, &sample);
	}
	int tex1 = m_shader->AddUniform("u_col_lut", ur::UNIFORM_INT1);
	if (tex1 >= 0) {
		float sample = 1;
		m_shader->SetUniform(tex1, ur::UNIFORM_INT1, &sample);
	}
}

void ColGradingProg::Bind()
{
	m_rc->BindTexture(m_lut_tex, 1);
}

void ColGradingProg::SetLUTTex(int tex)
{
	m_lut_tex = tex;
	m_rc->BindTexture(m_lut_tex, 1);
}

}