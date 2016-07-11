#include "Shader.h"
#include "Uniform.h"

#include <ee/IShader.h>
#include <ee/std_functor.h>

#include <algorithm>
#include <fstream>
#include <sstream>

namespace eshader
{

Shader::Shader()
	: m_shader_impl(NULL)
{
}

Shader::~Shader()
{
	delete m_shader_impl;
	for_each(m_uniforms.begin(), m_uniforms.end(), ee::DeletePointerFunctor<Uniform>());
}

void Shader::LoadShader()
{
	m_shader_impl->Load();
}

void Shader::LoadUniforms()
{
	BindShader();
	for (int i = 0, n = m_uniforms.size(); i < n; ++i) {
		m_uniforms[i]->Load();
	}
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
ReadFromFile(const std::string& filepath, std::string& output)
{
	std::locale::global(std::locale(""));	
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));

	std::stringstream buffer;
	buffer << fin.rdbuf();

	output.assign(buffer.str());
}

}