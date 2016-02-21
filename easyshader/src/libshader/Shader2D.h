#ifndef _EASYSHADER_SHADER2D_H_
#define _EASYSHADER_SHADER2D_H_

#include "Shader.h"

#include <ee/SpriteShader.h>

namespace eshader 
{

class Shader2D : public Shader
{
public:
	Shader2D(const std::string& vert_filepath, const std::string& frag_filepath);
	virtual ~Shader2D();

	virtual void BindShader();

	void SetTimeUniform(float time);
	void SetInputUniform(float x, float y);

	void AddTimeUniform(Uniform* uniform);
	void AddInputUniform(Uniform* uniform);

private:
	class ShaderImpl : public ee::SpriteShader, private Shader::ShaderImpl
	{
	public:
		ShaderImpl(const std::string& vert_filepath, const std::string& frag_filepath);

	protected:
		virtual void LoadShader();

	}; // ShaderImpl

private:
	Uniform* m_time_uniform;
	Uniform* m_input_uniform;

}; // Shader2D

}

#endif // _EASYSHADER_SHADER2D_H_