#include <gl/glew.h>

#include "Shader3D.h"

namespace eshader
{

Shader3D::Shader3D(const std::string& vert_filepath, const std::string& frag_filepath)
{
	m_shader_impl = new Shader3D::ShaderImpl(vert_filepath, frag_filepath);
}

void Shader3D::BindShader()
{
	e3d::ShaderMgr::Instance()->Model();
}

//////////////////////////////////////////////////////////////////////////
// class Shader3D::ShaderImpl
//////////////////////////////////////////////////////////////////////////

Shader3D::ShaderImpl::
ShaderImpl(const std::string& vert_filepath, const std::string& frag_filepath)
	: Shader::ShaderImpl(vert_filepath, frag_filepath)
{
}

void Shader3D::ShaderImpl::
LoadShader()
{
	InitShader(m_vert.c_str(), m_frag.c_str());
	m_normal_matrix = glGetUniformLocation(m_prog, "u_normal_matrix");
}

void Shader3D::ShaderImpl::
SetNormalMatrix(const ee::mat3& noraml_mat)
{
	glUniformMatrix3fv(m_normal_matrix, 1, 0, noraml_mat.Pointer());
}

}