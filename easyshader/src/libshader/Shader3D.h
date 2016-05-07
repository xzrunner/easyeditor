#ifndef _EASYSHADER_SHADER3D_H_
#define _EASYSHADER_SHADER3D_H_

#include "Shader.h"

#include <easy3d.h>
#include <SM_Matrix.h>

namespace eshader 
{

class Shader3D : public Shader
{
public:
	Shader3D(const std::string& vert_filepath, const std::string& frag_filepath);

	virtual void BindShader();

private:
	class ShaderImpl : public e3d::ModelShader, private Shader::ShaderImpl
	{
	public:
		ShaderImpl(const std::string& vert_filepath, const std::string& frag_filepath);

	protected:
		virtual void LoadShader();

		virtual void SetNormalMatrix(const sm::mat3& noraml_mat);

	private:
		GLuint m_normal_matrix;

	}; // ShaderImpl

}; // Shader3D

}

#endif // _EASYSHADER_SHADER3D_H_