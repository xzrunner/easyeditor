#include "Shader.h"

#include <GL/GLee.h>
#include <stdio.h>

namespace d2d
{

Shader* Shader::m_instance = NULL;

Shader* Shader::Instance()
{
	if (!m_instance)
	{
		m_instance = new Shader();
		m_instance->load();
	}
	return m_instance;
}

Shader::Shader()
{
	m_prog_curr = 0;
	m_prog_sprite = 0;
	m_prog_shape = 0;
}

void Shader::color(const Colorf& multi, const Colorf& add)
{
	if (m_prog_curr == m_prog_sprite) {
		glUniform4fv(m_multi_loc, 1, (GLfloat*)(&multi.r));
		glUniform4fv(m_add_loc, 1, (GLfloat*)(&add.r));
	} else {
		glUniform4fv(m_col_loc, 1, (GLfloat*)(&multi.r));
	}
}

void Shader::color(float r, float g, float b, float a)
{
	if (m_prog_curr == 0)
	{
		glColor4f(r, g, b, a);
	}
	else
	{
		float mul[4] = {r, g, b, a},
			add[4] = {0, 0, 0, 0};
		if (m_prog_curr == m_prog_sprite) {
			glUniform4fv(m_multi_loc, 1, mul);
			glUniform4fv(m_add_loc, 1, add);
		} else {
			glUniform4fv(m_col_loc, 1, mul);
		}
	}
}

void Shader::sprite()
{
	if (m_prog_curr != m_prog_sprite) {
		glEnable(GL_BLEND);

		// todo 源混合因子ejoy2d用的GL_ONE
//		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(m_prog_sprite);
		m_prog_curr = m_prog_sprite;

		color(1, 1, 1, 1);
	}
}

void Shader::shape()
{
	if (m_prog_curr != m_prog_shape) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(m_prog_shape);
		m_prog_curr = m_prog_shape;
	}
}

void Shader::null()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(0);
	m_prog_curr = 0;
}

void Shader::release()
{
	unload();
}

void Shader::reload()
{
	load();
}

void Shader::load()
{
#if 0
#define FLOAT_PRECISION \
	"#ifdef GL_FRAGMENT_PRECISION_HIGH  \n" \
	"precision highp float;  \n" \
	"#else  \n" \
	"precision lowp float;  \n" \
	"#endif  \n"
#else
#define FLOAT_PRECISION \
  "precision lowp float;  \n"
#endif

	static const char * sprite_vs =
		FLOAT_PRECISION
		"uniform vec4 color;     \n"
		"uniform vec4 additive;     \n"
		"\n"
		"varying vec2 v_texcoord;  \n"
		"varying vec4 v_fragmentColor;  \n"
		"varying vec4 v_fragmentAddi; \n"
		"\n"
		"void main()  \n"
		"{  \n"
		"  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; "
		"  v_fragmentColor = color; \n"
		"  v_fragmentAddi = additive; \n"
		"  v_texcoord = vec2(gl_MultiTexCoord0);  \n"
		"}  \n"
		;

	static const char * sprite_fs =
		FLOAT_PRECISION
		"varying vec4 v_fragmentColor; \n"
		"varying vec4 v_fragmentAddi; \n"
		"varying vec2 v_texcoord;  \n"
		"uniform sampler2D texture0;  \n"
		"\n"
		"void main()  \n"
		"{  \n"  
 		"  vec4 tmp = texture2D(texture0, v_texcoord);  \n"
 		"  gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
 		"  gl_FragColor.w = tmp.w;    \n"
 		"  gl_FragColor *= v_fragmentColor.w;  \n"
 		"  gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
		"}  \n"
		;

	static const char* font_fs = 
		FLOAT_PRECISION
		"varying vec4 v_fragmentColor; \n"
		"varying vec2 v_texcoord;  \n"
		"uniform sampler2D texture0;  \n"
		"\n"
		"void main()  \n"
		"{  \n"  
		"  float mask = texture2D(texture0, v_texcoord).a;    \n"
		"  vec4 clr;                                          \n"
		"  clr.rgb = vec3(1.0, 0.0, 0.0);                     \n"
		"  if (mask < 0.5)				                      \n"
		"    clr.a = 0.0;                                     \n"
		"  else                                               \n"
		"    clr.a = 1.0;                                     \n"
		"  gl_FragColor = clr;                                \n"
		"}  \n"
		;

	static const char * shape_vs =
		FLOAT_PRECISION
		"uniform vec4 color;     \n"
		"\n"
		"varying vec4 v_fragmentColor;  \n"
		"\n"
		"void main()  \n"
		"{  \n"
		"  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; "
		"  v_fragmentColor = color; \n"
		"}  \n"
		;

	static const char * shape_fs =
		FLOAT_PRECISION
		"varying vec4 v_fragmentColor; \n"
		"\n"
		"void main()  \n"
		"{  \n"  
		"  gl_FragColor = v_fragmentColor;"
		"}  \n"
		;


	glEnable(GL_BLEND);
	// todo 源混合因子ejoy2d用的GL_ONE
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_prog_sprite = init(sprite_fs, sprite_vs);
	m_prog_shape = init(shape_fs, shape_vs);
	m_prog_font = init(font_fs, sprite_vs);

	// bind attr
	m_multi_loc = glGetUniformLocation(m_prog_sprite, "color");
	m_add_loc = glGetUniformLocation(m_prog_sprite, "additive");
	m_col_loc = glGetUniformLocation(m_prog_shape, "color");
}

void Shader::unload()
{
	delete m_instance;
	m_instance = NULL;
}

int Shader::init(const char *FS, const char *VS)
{
	GLuint prog = glCreateProgram();

	GLuint fs = compile(FS, GL_FRAGMENT_SHADER);
	if (fs == 0) {
		return 0;
	} else {
		glAttachShader(prog, fs);
	}

	GLuint vs = compile(VS, GL_VERTEX_SHADER);
	if (vs == 0) {
		return 0;
	} else {
		glAttachShader(prog, vs);
	}

	// link
	GLint status;
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == 0) {
		return 0;
	}

	// detatch
	glDetachShader(prog, fs);
	glDeleteShader(fs);
	glDetachShader(prog, vs);
	glDeleteShader(vs);

	return prog;
}

int Shader::compile(const char * source, int type)
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