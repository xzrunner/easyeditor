#include "gl/Shader.h"

#include <ejoy2d/render.h>

namespace ur
{
namespace gl
{

Shader::Shader(render* render)
	: m_render(render)
{
}

int Shader::Create(const char* vs, const char* fs)
{
	struct shader_init_args args;
	args.vs = vs;
	args.fs = fs;
	args.texture = 0;
	return render_shader_create(m_render, &args);
}

void Shader::Release(int id)
{
	render_release(m_render, SHADER, id);
}

void Shader::Bind(int id)
{
	render_shader_bind(m_render, id);
}

int Shader::GetUniform(const char* name)
{
	return render_shader_locuniform(m_render, name);
}

void Shader::SetUniform(int loc, enum UNIFORM_FORMAT format, const float *v)
{
	render_shader_setuniform(m_render, loc, format, v);
}

}
}