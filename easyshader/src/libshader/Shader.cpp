#include "Shader.h"
#include "Uniform.h"

namespace eshader
{

Shader::Shader(const std::string& vert_filepath, const std::string& frag_filepath)
	: m_shader_impl(new ShaderImpl(vert_filepath, frag_filepath))
{
	
}

Shader::~Shader()
{
	delete m_shader_impl;
	for_each(m_uniforms.begin(), m_uniforms.end(), DeletePointerFunctor<Uniform>());
}

bool Shader::Load()
{
	m_shader_impl->Load();
	return true;
}

void Shader::AddUniform(Uniform* uniform)
{
	m_uniforms.push_back(uniform);
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