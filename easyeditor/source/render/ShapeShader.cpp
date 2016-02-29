#include "ShapeShader.h"

#define STRINGIFY(A)  #A
#include "ShapeShader.vert"
#include "ShapeShader.frag"

#include <sl_shape.h>

#include <gl/glew.h>

#include <string>

namespace ee
{

ShapeShader::ShapeShader()
{
	m_model_view = m_projection = 0;
	m_color_loc = 0;
	m_is_mat_dirty = false;
}

void ShapeShader::Load()
{
	//////////////////////////////////////////////////////////////////////////
	sl_shape_load();
	sl_shape_modelview(0, 0, 1, 1);
	//////////////////////////////////////////////////////////////////////////

// 	LoadShader();
// 
// 	m_projection = glGetUniformLocation(m_prog, "u_projection");
// 	m_model_view = glGetUniformLocation(m_prog, "u_modelview");
// 
// 	m_color_loc = glGetUniformLocation(m_prog, "color");
}

void ShapeShader::Unload()
{
	//////////////////////////////////////////////////////////////////////////
	sl_shape_unload();
	//////////////////////////////////////////////////////////////////////////
}

void ShapeShader::Bind()
{
	//////////////////////////////////////////////////////////////////////////
	sl_shape_bind();
	//////////////////////////////////////////////////////////////////////////

// 	glUseProgram(m_prog);
// 
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ShapeShader::Unbind()
{
	//////////////////////////////////////////////////////////////////////////
	sl_shape_unbind();
	//////////////////////////////////////////////////////////////////////////
}

void ShapeShader::SetModelView(const Vector& offset, float scale)
{
	//////////////////////////////////////////////////////////////////////////
	sl_shape_modelview(offset.x, offset.y, scale, scale);
	//////////////////////////////////////////////////////////////////////////

// 	m_mat_modelview.SetScale(scale, scale);
// 	m_mat_modelview.Translate(offset.x, offset.y);
// 	m_is_mat_dirty = true;
}

void ShapeShader::SetProjection(int width, int height)
{
	//////////////////////////////////////////////////////////////////////////
	sl_shape_projection(width, height);
	//////////////////////////////////////////////////////////////////////////

// 	float hw = width * 0.5f;
// 	float hh = height * 0.5f;
// 	m_mat_projection.Orthographic(-hw, hw, -hh, hh, 1, -1);
// 	m_is_mat_dirty = true;
}

void ShapeShader::Commit()
{
// 	if (m_is_mat_dirty) {
// 		glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.GetElements());
// 		glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.GetElements());
// 		m_is_mat_dirty = false;
// 	}
}

void ShapeShader::SetColor(const Colorf& col)
{
	//////////////////////////////////////////////////////////////////////////
	//sl_shape_color();
	//////////////////////////////////////////////////////////////////////////

// 	m_color = col;
// 	glUniform4fv(m_color_loc, 1, (GLfloat*)(&col.r));
}

void ShapeShader::LoadShader()
{
// 	static const std::string header(FLOAT_PRECISION);
// 	static const std::string vert(header + ShapeVertShader);
// 	static const std::string frag(header + ShapeFragShader);
// 
// 	InitShader(vert.c_str(), frag.c_str());
}

}