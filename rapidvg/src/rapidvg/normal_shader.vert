static const char* normal_vert = STRINGIFY(

attribute vec4 position;
attribute vec4 color;   

varying vec4 v_color;

uniform mat4 u_projection;
uniform mat4 u_modelview;

void main()
{
	gl_Position = u_projection * u_modelview * position;
	v_color = color / 255.0;
}

);