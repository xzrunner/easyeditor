#include "FontShader.h"

namespace d2d
{

FontShader::FontShader()
{

}

void FontShader::Load()	
{
	// same with sprite's
	static const char* vs =
		FLOAT_PRECISION
		"attribute vec4 position;  \n"
		"attribute vec2 texcoord;  \n"
		"attribute vec4 color;     \n"
		"attribute vec4 additive;  \n"
		"\n"
		"varying vec2 v_texcoord;  \n"
		"varying vec4 v_fragmentColor;  \n"
		"varying vec4 v_fragmentAddi; \n"
		"\n"
		"uniform mat4 u_projection; \n"
		"uniform mat4 u_modelview; \n"
		"\n"
		"void main()  \n"
		"{  \n"
		"  gl_Position = u_projection * u_modelview * position; "
		"  v_fragmentColor = color / 255.0; \n"
		"  v_fragmentAddi = additive / 255.0; \n"
		"  v_texcoord = texcoord;  \n"
		"}  \n"
		;

	static const char* fs = 
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
}

void FontShader::Unload()
{

}

}