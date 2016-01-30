#include "ScreenShader.h"

#include <gl/glew.h>

namespace ee
{

static const float VERTICES[] = { -1, -1, -1, 1, 1, 1, 1, -1 };
static const float TEXCOORDS[] = { 0, 0, 0, 1, 1, 1, 1, 0 };

ScreenShader::ScreenShader()
{

}

ScreenShader::~ScreenShader()
{

}

void ScreenShader::Load()
{
	static const char* vs =
		FLOAT_PRECISION
		"varying vec2 v_texcoord;  \n"
		"void main()  \n"
		"{  \n"
		"  gl_Position = gl_Vertex; "
		"  v_texcoord = gl_MultiTexCoord0.xy;  \n"
		"}  \n"
		;

	static const char* fs =
		FLOAT_PRECISION
		"varying vec2 v_texcoord;  \n"
		"uniform sampler2D texture0;  \n"
		"\n"
		"void main()  \n"
		"{  \n"  
		"	 gl_FragColor = texture2D(texture0, v_texcoord);  \n"
		"}  \n"
		;

	InitShader(vs, fs);
}

void ScreenShader::Unload()
{
}

void ScreenShader::Bind()
{
	glUseProgram(m_prog);
}

void ScreenShader::Unbind()
{
}

void ScreenShader::SetProjection(int width, int height)
{
}

void ScreenShader::Commit()
{
}

void ScreenShader::Draw(int texid) const
{
	glBindTexture(GL_TEXTURE_2D, texid);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, (const GLvoid*)VERTICES);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, (const GLvoid*)TEXCOORDS);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

}