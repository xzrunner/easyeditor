#ifndef _ESHADER_SHADER_H_
#define _ESHADER_SHADER_H_

#include <drag2d.h>

namespace eshader 
{

class Uniform;

class Shader
{
public:
	Shader();
	virtual ~Shader();

	virtual void BindShader() = 0;

	void LoadShader();

	void LoadUniforms();

	void AddUniform(Uniform* uniform);

	d2d::IShader* GetShaderImpl() { return m_shader_impl; }

protected:
	class ShaderImpl
	{
	public:
		ShaderImpl(const std::string& vert_filepath, const std::string& frag_filepath);

	private:
		static void ReadFromFile(const std::string& filepath, std::string& output);

	protected:
		std::string m_vert, m_frag;

	}; // ShaderImpl

protected:
	d2d::IShader* m_shader_impl;

private:
	std::vector<Uniform*> m_uniforms;

}; // Shader

}

#endif // _ESHADER_SHADER_H_