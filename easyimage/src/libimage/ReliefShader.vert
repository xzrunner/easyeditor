static const char* ReliefVertShader = STRINGIFY(

attribute vec4 position;
attribute vec2 texcoord;
attribute vec4 color;   
attribute vec4 additive;

varying vec2 v_texcoord;
varying vec4 v_fragmentColor;
varying vec4 v_fragmentAddi;

uniform mat4 u_projection;
uniform mat4 u_modelview;

void main()
{
  gl_Position = u_projection * u_modelview * position;
  v_fragmentColor = color / 255.0;
  v_fragmentAddi = additive / 255.0;
  v_texcoord = texcoord;
}

);
