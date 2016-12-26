#include "BurningMapProg.h"
#include "RenderShader.h"
#include "BurningMap.h"

#include <unirender/RenderContext.h>

namespace sl
{

BurningMapProg::BurningMapProg(ur::RenderContext* rc, int max_vertex, 
							   const std::vector<ur::VertexAttrib>& va_list, 
							   RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
	, m_height_map_tex(0)
	, m_upper_tex(0)
{
	Init(va_list, ib, new parser::BurningMap());

	m_lifetime = m_shader->AddUniform("u_lifetime", ur::UNIFORM_FLOAT1);
	m_time = m_shader->AddUniform("u_time", ur::UNIFORM_FLOAT1);

	// lower
	int tex0 = m_shader->AddUniform("u_texture0", ur::UNIFORM_INT1);
	if (tex0 >= 0) {
		float sample = 0;
		m_shader->SetUniform(tex0, ur::UNIFORM_INT1, &sample);
	}
	// upper
	int tex1 = m_shader->AddUniform("u_texture1", ur::UNIFORM_INT1);
	if (tex1 >= 0) {
		float sample = 1;
		m_shader->SetUniform(tex1, ur::UNIFORM_INT1, &sample);
	}
	// height map
	int tex2 = m_shader->AddUniform("u_height_map_tex", ur::UNIFORM_INT1);
	if (tex2 >= 0) {
		float sample = 2;
		m_shader->SetUniform(tex2, ur::UNIFORM_INT1, &sample);
	}
	// border gradient
	int tex3 = m_shader->AddUniform("u_border_gradient_tex", ur::UNIFORM_INT1);
	if (tex3 >= 0) {
		float sample = 3;
		m_shader->SetUniform(tex3, ur::UNIFORM_INT1, &sample);
	}
}

void BurningMapProg::UpdateTime(float time)
{
	m_shader->SetUniform(m_time, ur::UNIFORM_FLOAT1, &time);
}

void BurningMapProg::SetLifeTime(float lifetime)
{
	m_shader->SetUniform(m_lifetime, ur::UNIFORM_FLOAT1, &lifetime);	
}

void BurningMapProg::SetUpperTex(int tex)
{
	m_upper_tex = tex;
	m_rc->BindTexture(m_upper_tex, 1);
}

void BurningMapProg::SetHeightMapTex(int tex)
{
	m_height_map_tex = tex;
	m_rc->BindTexture(m_height_map_tex, 2);
}

void BurningMapProg::SetBorderGradientTex(int tex)
{
	m_border_gradient_tex = tex;
	m_rc->BindTexture(m_border_gradient_tex, 3);
}

}