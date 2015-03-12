#include "ShapeShader.h"

#include "common/Color.h"

#include <gl/GLee.h>

namespace e2d
{

ShapeShader::ShapeShader()
{
	m_model_view = m_projection = 0;
	m_color = 0;
	m_is_mat_dirty = false;
}

void ShapeShader::Load()
{
	static const char* vs =
		FLOAT_PRECISION
		"uniform vec4 color;     \n"
		"\n"
		"varying vec4 v_fragmentColor;  \n"
		"\n"
		"\n"
		"uniform mat4 u_projection; \n"
		"uniform mat4 u_modelview; \n"
		"\n"
		"void main()  \n"
		"{  \n"
		"  gl_Position = u_projection * u_modelview * gl_Vertex; "
		"  v_fragmentColor = color; \n"
		"}  \n"
		;

	static const char* fs =
		FLOAT_PRECISION
		"varying vec4 v_fragmentColor; \n"
		"\n"
		"void main()  \n"
		"{  \n"  
		"  gl_FragColor = v_fragmentColor;"
		"}  \n"
		;

// 	glEnable(GL_BLEND);
// 	// todo 源混合因子ejoy2d用的GL_ONE
// 	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	InitShader(vs, fs);

	m_projection = glGetUniformLocation(m_prog, "u_projection");
	m_model_view = glGetUniformLocation(m_prog, "u_modelview");

	m_color = glGetUniformLocation(m_prog, "color");
}

void ShapeShader::Unload()
{
}

void ShapeShader::Bind()
{
	glUseProgram(m_prog);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ShapeShader::Unbind()
{
}

void ShapeShader::SetModelView(const vec2& offset, float scale)
{
	m_mat_modelview.SetScale(scale, scale);
	m_mat_modelview.Translate(offset.x, offset.y);
	m_is_mat_dirty = true;
}

void ShapeShader::SetProjection(int width, int height)
{
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	m_mat_projection.Orthographic(-hw, hw, -hh, hh, 1, -1);
	m_is_mat_dirty = true;
}

void ShapeShader::Commit()
{
	if (m_is_mat_dirty) {
		glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.GetElements());
		glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.GetElements());
		m_is_mat_dirty = false;
	}
}

void ShapeShader::SetColor(const Colorf& col)
{
	glUniform4fv(m_color, 1, (GLfloat*)(&col.r));
}

}