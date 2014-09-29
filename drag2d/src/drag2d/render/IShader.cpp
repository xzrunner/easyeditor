#include "IShader.h"

#include <gl/GLee.h>
#include <stdio.h>

namespace d2d
{

IShader::IShader()
{
	m_prog = 0;
}

void IShader::InitShader(const char* FS, const char* VS)
{
	GLuint prog = glCreateProgram();

	GLuint fs = CompileShader(FS, GL_FRAGMENT_SHADER);
	if (fs == 0) {
		return;
	} else {
		glAttachShader(prog, fs);
	}

	GLuint vs = CompileShader(VS, GL_VERTEX_SHADER);
	if (vs == 0) {
		return;
	} else {
		glAttachShader(prog, vs);
	}

	BindAttrib(prog);

	// link
	GLint status;
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == 0) {
		return;
	}

	// detatch
	glDetachShader(prog, fs);
	glDeleteShader(fs);
	glDetachShader(prog, vs);
	glDeleteShader(vs);

	m_prog = prog;
}

GLuint IShader::CompileShader(const char* source, GLuint type)
{
	GLint status;

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		char buf[1024];
		GLint len;
		glGetShaderInfoLog(shader, 1024, &len, buf);

		printf("compile failed:%s\n"
			"source:\n %s\n",
			buf, source);
		glDeleteShader(shader);
		return 0;
	}

	return shader;	
}

}