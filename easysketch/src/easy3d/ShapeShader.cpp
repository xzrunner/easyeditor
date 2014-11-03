#include "Matrix.h"

#define NOMINMAX 
#include <gl/GLee.h>

#include "ShapeShader.h"
#include "Camera.h"

namespace e3d
{

ShapeShader::ShapeShader()
{
	m_model_view = m_projection = 0;
	m_color = 0;
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

	InitShader(fs, vs);

	m_projection = glGetUniformLocation(m_prog, "u_projection");
	m_model_view = glGetUniformLocation(m_prog, "u_modelview");

	m_color = glGetUniformLocation(m_prog, "color");

	// Set up transforms.
	m_translation = mat4::Translate(0, 0, -1005);

	m_mat_modelview = mat4::Translate(0, 0, -1005);
}

void ShapeShader::Unload()
{
}

void ShapeShader::Bind()
{
	glUseProgram(m_prog);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

// 	glEnable(GL_LINE_SMOOTH);
// 	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);                 // Set Line Antialiasing
}

void ShapeShader::Unbind()
{
	glDisable(GL_DEPTH_TEST);
}

void ShapeShader::SetModelView(const mat4& mat)
{
	m_mat_modelview = mat;
}

void ShapeShader::SetProjection(int width, int height)
{
	float hh = 1.0f * height / width;
	m_mat_projection = mat4::Perspective(-1, 1, -hh, hh, 
		Camera::CAM_NEAR, Camera::CAM_FAR);
}

void ShapeShader::Commit()
{
	glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.Pointer());
	glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.Pointer());
}

void ShapeShader::SetColor(const d2d::Colorf& col)
{
	glUniform4fv(m_color, 1, (GLfloat*)(&col.r));
}

}