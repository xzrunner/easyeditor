#ifndef _DRAG2D_SHADER_IMPL_H_
#define _DRAG2D_SHADER_IMPL_H_

#include "common/Color.h"
#include "common/tools.h"
#include "common/Matrix.h"

typedef unsigned int GLuint;

namespace d2d
{

class ShaderImpl
{
public:
	ShaderImpl();

private:
	class RenderState
	{
	public:
		RenderState() {
			prog = 0;
			model_view = projection = 0;
			is_mat_dirty = false;
		}

	public:
		GLuint prog;
		GLuint model_view, projection;

		bool is_mat_dirty;
	}; // RenderState

 	class SpriteRS : public RenderState
 	{
 	public:
 
 	public:
 
 	}; // SpriteRS
 
 	class ShapeRS : public RenderState
 	{
 	public:
 
 	public:
 
 	}; // ShapeRS

private:
	static int MAX_COMMBINE;
	static const int SPRITE_FLOAT_NUM = 24;	// 

private:
	GLuint m_prog_curr;

	GLuint m_prog_font;

	SpriteRS m_rs_sprite;
	ShapeRS m_rs_shape;

	GLuint VertexBuffer;
	GLuint IndexBuffer;

	int m_sprite_count;

	float* m_vb;

	GLuint m_tex;
	GLuint m_fbo;

	uint32_t m_color, m_additive;

	// for shape
	int m_col_loc;

	int m_version;

	bool m_open_buffer_data;

	Matrix m_mat_modelview, m_mat_projection;

	friend class ShaderNew;

}; // ShaderImpl

}

#endif // _DRAG2D_SHADER_IMPL_H_