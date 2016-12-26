#ifndef _SHADERLAB_SHADER_PROGRAM_H_
#define _SHADERLAB_SHADER_PROGRAM_H_

#include <unirender/VertexAttrib.h>

#include <vector>

namespace ur { class RenderContext; }

namespace sl
{

namespace parser { class Shader; class Node; }

class RenderShader;
class RenderBuffer;
class ObserverMVP;

class ShaderProgram
{
public:
	ShaderProgram(ur::RenderContext* rc, int max_vertex);
	virtual ~ShaderProgram();

	virtual void Bind() {}

	void Load(parser::Node* vert, parser::Node* frag, 
		const std::vector<ur::VertexAttrib>& va_list, 
		RenderBuffer* ib, bool has_mvp);

	RenderShader* GetShader() { return m_shader; }
	int GetVertexSize() const { return m_vertex_sz; }
	ObserverMVP* GetMVP() const { return m_mvp; }

private:
	void Release();

protected:
	ur::RenderContext* m_rc;

	RenderShader* m_shader;

private:
	int m_max_vertex;

	parser::Shader* m_parser;
	int m_vertex_sz;

	ObserverMVP* m_mvp;

}; // ShaderProgram

}

#endif // _SHADERLAB_SHADER_PROGRAM_H_