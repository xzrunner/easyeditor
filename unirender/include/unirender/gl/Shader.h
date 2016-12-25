#ifndef _UNIRENDER_GL_SHADER_H_
#define _UNIRENDER_GL_SHADER_H_

#include <ejoy2d/render.h>

#include <CU_Uncopyable.h>

namespace ur
{
namespace gl
{

class Shader : private cu::Uncopyable
{
public:
	Shader(render* render);

	int  Create(const char* vs, const char* fs);
	void Release(int id);

	void Bind(int id);

	int  GetUniform(const char* name);
	void SetUniform(int loc, enum UNIFORM_FORMAT format, const float *v);
	
private:
	render* m_render;

}; // Shader

}
}

#endif // _UNIRENDER_GL_SHADER_H_