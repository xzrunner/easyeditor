#ifndef _SHADERLAB_MASK_SHADER_H_
#define _SHADERLAB_MASK_SHADER_H_

#include "Shader.h"
#include "ShaderProgram.h"

#include <vector>

namespace sl
{

class MaskShader : public Shader
{
public:
	MaskShader(ur::RenderContext* rc);
	virtual ~MaskShader();

	virtual void Bind() const;
	virtual void UnBind() const;
	virtual void Commit() const;

	void Draw(const float* positions, const float* texcoords, 
		const float* texcoords_mask, int tex, int tex_mask) const;

private:
	void InitVAList();
	void InitProg();

private:
	enum VA_TYPE {
		POSITION = 0,
		TEXCOORD,
		TEXCOORD_MASK,
		VA_MAX_COUNT
	};

	struct Vertex
	{
		float vx, vy;
		float tx, ty;
		float tx_mask, ty_mask;
	};

	class Program : public ShaderProgram
	{
	public:
		Program(ur::RenderContext* rc, const std::vector<ur::VertexAttrib>& va_list, 
			RenderBuffer* ib);
	private:
		void Init(const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib);
	}; // Program

private:
	ur::VertexAttrib m_va_list[VA_MAX_COUNT];

	ShaderProgram* m_prog;

	mutable int m_tex, m_tex_mask;

	Vertex* m_vertex_buf;
	mutable int m_quad_sz;

}; // MaskShader

}

#endif // _SHADERLAB_MASK_SHADER_H_