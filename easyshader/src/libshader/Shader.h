#ifndef _ESHADER_SHADER_H_
#define _ESHADER_SHADER_H_

//#define IS_2D

#include <string>
#include <drag2d.h>
#ifndef IS_2D
#include <easy3d.h>
#endif

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
	void SetTimeUniform(float time);
	void SetInputUniform(float x, float y);

	void AddUniform(Uniform* uniform);
	void AddTimeUniform(Uniform* uniform);
	void AddInputUniform(Uniform* uniform);

#ifdef IS_2D
	d2d::SpriteShader* GetShaderImpl() { return m_shader_impl; }
#else
	e3d::ModelShader* GetShaderImpl() { return m_shader_impl; }
#endif

private:
#ifdef IS_2D
	class ShaderImpl : public d2d::SpriteShader
#else
	class ShaderImpl : public e3d::ModelShader
#endif
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
#ifdef IS_2D
	d2d::SpriteShader* m_shader_impl;
#else
	e3d::ModelShader* m_shader_impl;
#endif

	// normal
	std::vector<Uniform*> m_uniforms;

	// special
	Uniform* m_time_uniform;
	Uniform* m_input_uniform;

}; // Shader

}

#endif // _ESHADER_SHADER_H_