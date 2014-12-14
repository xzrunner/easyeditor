#define NOMINMAX
#include <gl/gLee.h>

#include "Shader.h"
#include "Uniform.h"

namespace eshader
{

Shader::Shader(const std::string& vert_filepath, const std::string& frag_filepath)
	: m_shader_impl(new ShaderImpl(vert_filepath, frag_filepath))
	, m_time_uniform(NULL)
{
	
}

Shader::~Shader()
{
	delete m_shader_impl;

	for_each(m_uniforms.begin(), m_uniforms.end(), DeletePointerFunctor<Uniform>());
	delete m_time_uniform;
}

void Shader::Load()
{
	m_shader_impl->Load();
}

void Shader::LoadUniforms()
{
	d2d::ShaderMgr::Instance()->sprite();
	for (int i = 0, n = m_uniforms.size(); i < n; ++i) {
		m_uniforms[i]->Load();
	}

	int loc_center = glGetUniformLocation(m_shader_impl->GetProgram(), "u_center");	
	float val[3];
	val[0] = val[1] = 0.5f;
	glUniform2fv(loc_center, 1, (float*) &val[0]); 
}

void Shader::UpdateTimeUniform(float time)
{
	if (m_time_uniform) {
		m_time_uniform->Set(time);
		m_time_uniform->Load();
	}
}

void Shader::AddUniform(Uniform* uniform)
{
	m_uniforms.push_back(uniform);
}

void Shader::SetTimeUniform(Uniform* uniform)
{
	m_time_uniform = uniform;
}

//////////////////////////////////////////////////////////////////////////
// class Shader::ShaderImpl
//////////////////////////////////////////////////////////////////////////

Shader::ShaderImpl::
ShaderImpl(const std::string& vert_filepath, const std::string& frag_filepath)
{
	ReadFromFile(vert_filepath, m_vert);
	ReadFromFile(frag_filepath, m_frag);
}

void Shader::ShaderImpl::
LoadShader()
{
	InitShader(m_vert.c_str(), m_frag.c_str());
}

void Shader::ShaderImpl::
ReadFromFile(const std::string& filepath, std::string& output)
{
	std::ifstream fin(filepath.c_str());

	std::stringstream buffer;
	buffer << fin.rdbuf();

	output.assign(buffer.str());
}

}