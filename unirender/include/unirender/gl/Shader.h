#ifndef _UNIRENDER_GL_SHADER_H_
#define _UNIRENDER_GL_SHADER_H_

#include <CU_Uncopyable.h>

struct render;

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
	
private:
	render* m_render;

}; // Shader

}
}

#endif // _UNIRENDER_GL_SHADER_H_