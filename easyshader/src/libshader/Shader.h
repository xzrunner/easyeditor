#ifndef _EASYSHADER_SHADER_H_
#define _EASYSHADER_SHADER_H_

#include <string>
#include <vector>

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

private:
	std::vector<Uniform*> m_uniforms;

}; // Shader

}

#endif // _EASYSHADER_SHADER_H_