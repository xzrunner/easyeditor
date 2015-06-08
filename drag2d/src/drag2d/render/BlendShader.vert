static const char* BlendVertShader = STRINGIFY(

attribute vec4 position;
attribute vec2 texcoord;
attribute vec4 color;
attribute vec4 additive;
attribute vec4 r_trans;
attribute vec4 g_trans;
attribute vec4 b_trans;
attribute vec2 texcoord_base;

varying vec2 v_texcoord;
varying vec2 v_texcoord_base;
varying vec4 v_fragmentColor;
varying vec4 v_fragmentAddi;
varying vec4 v_r_trans;
varying vec4 v_g_trans;
varying vec4 v_b_trans;

uniform mat4 u_projection;
uniform mat4 u_modelview;

void main()
{
	gl_Position = u_projection * u_modelview * position;
	v_fragmentColor = color / 255.0;
	v_fragmentAddi = additive / 255.0;
	v_r_trans = r_trans / 255.0;
	v_g_trans = g_trans / 255.0;
	v_b_trans = b_trans / 255.0;
	v_texcoord = texcoord;
	v_texcoord_base = texcoord_base;
}

);