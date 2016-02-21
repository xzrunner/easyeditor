#ifndef _EASYSHADER_SHADER_H_
#define _EASYSHADER_SHADER_H_

#include <string>
#include <vector>

namespace ee { class IShader; }

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

	ee::IShader* GetShaderImpl() { return m_shader_impl; }

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
	ee::IShader* m_shader_impl;

private:
	std::vector<Uniform*> m_uniforms;

}; // Shader

}

#endif // _EASYSHADER_SHADER_H_