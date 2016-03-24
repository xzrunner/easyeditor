#include "Shader2D.h"
#include "Uniform.h"

#include <ee/ShaderMgr.h>

namespace eshader
{

Shader2D::Shader2D(const std::string& vert_filepath, const std::string& frag_filepath)
	: m_time_uniform(NULL)
	, m_input_uniform(NULL)
{
	m_shader_impl = new Shader2D::ShaderImpl(vert_filepath, frag_filepath);
}

Shader2D::~Shader2D()
{
	delete m_time_uniform;
	delete m_input_uniform;
}

void Shader2D::BindShader()
{
	ee::ShaderMgr::Instance()->SetShader(ee::ShaderMgr::SPRITE);
}

void Shader2D::SetTimeUniform(float time)
{
	if (m_time_uniform) {
		m_time_uniform->Set(time);
		m_time_uniform->Load();
	}
}

void Shader2D::SetInputUniform(float x, float y)
{
	if (m_input_uniform) {
		m_input_uniform->Set(x, y);
		m_input_uniform->Load();
	}
}

void Shader2D::AddTimeUniform(Uniform* uniform)
{
	m_time_uniform = uniform;
}

void Shader2D::AddInputUniform(Uniform* uniform)
{
	m_input_uniform = uniform;
}

//////////////////////////////////////////////////////////////////////////
// class Shader2D::ShaderImpl
//////////////////////////////////////////////////////////////////////////

Shader2D::ShaderImpl::
ShaderImpl(const std::string& vert_filepath, const std::string& frag_filepath)
	: Shader::ShaderImpl(vert_filepath, frag_filepath)
{
}

void Shader2D::ShaderImpl::
LoadShader()
{
	InitShader(m_vert.c_str(), m_frag.c_str());
}

}