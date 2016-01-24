static const char* sprite_vert = STRINGIFY(

attribute vec4 position;
attribute vec2 texcoord;
attribute vec4 color;   
attribute vec4 additive;
attribute vec4 rmap;
attribute vec4 gmap;
attribute vec4 bmap;

varying vec2 v_texcoord;
varying vec4 v_color;
varying vec4 v_additive;
varying vec4 v_rmap;
varying vec4 v_gmap;
varying vec4 v_bmap;

uniform mat4 u_projection;
uniform mat4 u_modelview;

void main()
{
	gl_Position = u_projection * u_modelview * position;
	v_color = color;
	v_additive = additive;
	v_texcoord = texcoord;
	v_rmap = rmap;
	v_gmap = gmap;
	v_bmap = bmap;
}

);