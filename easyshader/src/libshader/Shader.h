#ifndef _ESHADER_SHADER_H_
#define _ESHADER_SHADER_H_

#include <string>
#include <drag2d.h>

namespace eshader 
{

class Uniform;

class Shader
{
public:
	Shader(const std::string& vert_filepath, const std::string& frag_filepath);
	~Shader();

	void Load();

	void LoadUniforms();
	void UpdateTimeUniform(float time);

	void AddUniform(Uniform* uniform);
	void SetTimeUniform(Uniform* uniform);

	d2d::SpriteShader* GetShaderImpl() { return m_shader_impl; }

private:
	class ShaderImpl : public d2d::SpriteShader
	{
	public:
		ShaderImpl(const std::string& vert_filepath, const std::string& frag_filepath);

	protected:
		virtual void LoadShader();

	private:
		static void ReadFromFile(const std::string& filepath, std::string& output);

	private:
		std::string m_vert, m_frag;

	}; // ShaderImpl

private:
	d2d::SpriteShader* m_shader_impl;

	// normal
	std::vector<Uniform*> m_uniforms;

	// special
	Uniform* m_time_uniform;
//	Uniform* m_position;

}; // Shader

}

#endif // _ESHADER_SHADER_H_