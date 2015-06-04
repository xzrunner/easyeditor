#include "IShader.h"

#include "common/Exception.h"

#include <gl/glew.h>
#include <stdio.h>
#include <assert.h>

namespace d2d
{

IShader::IShader()
{
	m_prog = 0;
}

IShader::~IShader()
{
	DeleteShader();
}

void IShader::InitShader(const char* VS, const char* FS)
{
	GLuint prog = glCreateProgram();

	GLuint vs = CompileShader(VS, GL_VERTEX_SHADER);
	if (vs == 0) {
		return;
	} else {
		glAttachShader(prog, vs);
	}

	GLuint fs = CompileShader(FS, GL_FRAGMENT_SHADER);
	if (fs == 0) {
		return;
	} else {
		glAttachShader(prog, fs);
	}

//	BindAttrib(prog);

	// link
	GLint status;
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		char buf[1024];
		glGetProgramInfoLog(prog, 1024, NULL, buf);
		throw d2d::Exception("link failed:%s\n", buf);
	}

	// detatch
	glDetachShader(prog, fs);
	glDeleteShader(fs);
	glDetachShader(prog, vs);
	glDeleteShader(vs);

	m_prog = prog;

	BindAttrib(prog);
}

void IShader::DeleteShader()
{
	glUseProgram(0);
	glDeleteProgram(m_prog);
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
		glGetShaderInfoLog(shader, 1024, NULL, buf);
		glDeleteShader(shader);
		throw d2d::Exception("compile failed:%s\n, source:\n %s\n", buf, source);
	}

	return shader;	
}

}