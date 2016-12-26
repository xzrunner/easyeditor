#ifndef _SHADERLAB_SHADER_H_
#define _SHADERLAB_SHADER_H_

namespace ur { class RenderContext; }

namespace sl
{

class Shader
{
public:
	Shader(ur::RenderContext* rc) : m_rc(rc) {}
	virtual ~Shader() {}
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual void Commit() const = 0;

protected:
	ur::RenderContext* m_rc;

}; // Shader

}

#endif // _SHADERLAB_SHADER_H_