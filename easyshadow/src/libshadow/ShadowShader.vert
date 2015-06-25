static const char* ShadowVertShader = STRINGIFY(

uniform vec4 color;   

varying vec4 v_fragmentColor;

uniform mat4 u_projection;
uniform mat4 u_modelview;

void main()
{
	gl_Position = u_projection * u_modelview * gl_Vertex; 
	gl_FrontColor = gl_Color;
	v_fragmentColor = color;
}

);