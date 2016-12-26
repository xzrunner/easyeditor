#include "ShockWaveProg.h"
#include "RenderShader.h"
#include "ShockWave.h"

#include <unirender/typedef.h>

namespace sl
{

ShockWaveProg::ShockWaveProg(ur::RenderContext* rc, int max_vertex, 
							 const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::ShockWave());

	m_time = m_shader->AddUniform("u_time", ur::UNIFORM_FLOAT1);
	m_center = m_shader->AddUniform("u_center", ur::UNIFORM_FLOAT2);
	m_params = m_shader->AddUniform("u_params", ur::UNIFORM_FLOAT3);
}

void ShockWaveProg::UpdateTime(float time)
{
	m_shader->SetUniform(m_time, ur::UNIFORM_FLOAT1, &time);
}

void ShockWaveProg::SetCenter(float center[2])
{
	m_shader->SetUniform(m_center, ur::UNIFORM_FLOAT2, center);
}

void ShockWaveProg::SetFactor(float params[3])
{
	m_shader->SetUniform(m_params, ur::UNIFORM_FLOAT3, params);
}

}