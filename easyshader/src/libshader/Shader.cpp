#ifndef IS_2D
#define NOMINMAX 
#include <gl/gLee.h>
#endif

#include "Shader.h"
#include "Uniform.h"

namespace eshader
{

Shader::Shader(const std::string& vert_filepath, const std::string& frag_filepath)
	: m_shader_impl(new ShaderImpl(vert_filepath, frag_filepath))
	, m_time_uniform(NULL)
	, m_input_uniform(NULL)
{
}

Shader::~Shader()
{
	delete m_shader_impl;

	for_each(m_uniforms.begin(), m_uniforms.end(), DeletePointerFunctor<Uniform>());
	delete m_time_uniform;
	delete m_input_uniform;
}

void Shader::Load()
{
	m_shader_impl->Load();
}

void Shader::LoadUniforms()
{
#ifdef IS_2D
	d2d::ShaderMgr::Instance()->sprite();
#else
	e3d::ShaderMgr::Instance()->Model();
#endif
	for (int i = 0, n = m_uniforms.size(); i < n; ++i) {
		m_uniforms[i]->Load();
	}
}

void Shader::SetTimeUniform(float time)
{
	if (m_time_uniform) {
		m_time_uniform->Set(time);
		m_time_uniform->Load();
	}
}

void Shader::SetInputUniform(float x, float y)
{
	if (m_input_uniform) {
		m_input_uniform->Set(x, y);
		m_input_uniform->Load();
	}
}

void Shader::AddUniform(Uniform* uniform)
{
	m_uniforms.push_back(uniform);
}

void Shader::AddTimeUniform(Uniform* uniform)
{
	m_time_uniform = uniform;
}

void Shader::AddInputUniform(Uniform* uniform)
{
	m_input_uniform = uniform;
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

#ifndef IS_2D
	m_normal_matrix = glGetUniformLocation(m_prog, "u_normal_matrix");
#endif
}

#ifndef IS_2D
void Shader::ShaderImpl::
SetNormalMatrix(const mat3& noraml_mat)
{
	glUniformMatrix3fv(m_normal_matrix, 1, 0, noraml_mat.Pointer());
}
#endif

void Shader::ShaderImpl::
ReadFromFile(const std::string& filepath, std::string& output)
{
	std::ifstream fin(filepath.c_str());

	std::stringstream buffer;
	buffer << fin.rdbuf();

	output.assign(buffer.str());
}

}