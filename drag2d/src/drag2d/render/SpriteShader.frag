static const char* SpriteFragShader = STRINGIFY(

varying vec4 v_fragmentColor;
varying vec4 v_fragmentAddi;
varying vec4 v_r_trans;
varying vec4 v_g_trans;
varying vec4 v_b_trans;
varying vec2 v_texcoord; 
uniform sampler2D texture0; 

void main() 
{   
	vec4 tmp = texture2D(texture0, v_texcoord); 

	float s = 1.5;
	vec4 r = vec4(tmp.r, 0, 0, 1);
	vec4 g = vec4(0, tmp.g, 0, 1);
	vec4 b = vec4(0, 0, tmp.b, 1);
	if ((v_r_trans.r != 1 || v_r_trans.g != 0 || v_r_trans.b != 0) && tmp.r > tmp.g * s && tmp.r > tmp.b * s) {
		r = tmp.r * v_r_trans;
		g = tmp.g * v_r_trans;
		b = tmp.b * v_r_trans;
	}
	if ((v_g_trans.g != 1 || v_g_trans.r != 0 || v_g_trans.b != 0) && tmp.g > tmp.r * s && tmp.g > tmp.b * s) {
		g = tmp.g * v_g_trans;
		r = tmp.r * v_g_trans;
		b = tmp.b * v_g_trans;
	}
	if ((v_b_trans.b != 1 || v_b_trans.r != 0 || v_b_trans.g != 0) && tmp.b > tmp.r * s && tmp.b > tmp.g * s) {
		b = tmp.b * v_b_trans;
		r = tmp.r * v_b_trans;
		g = tmp.g * v_b_trans;
	}
	tmp.xyz = (r + g + b).xyz;

	gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz; 
	gl_FragColor.w = tmp.w;   
	gl_FragColor *= v_fragmentColor.w; 
	gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w; 
} 

);