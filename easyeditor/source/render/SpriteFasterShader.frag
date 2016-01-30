static const char* SpriteFasterFragShader = STRINGIFY(

varying vec4 v_fragmentColor;
varying vec4 v_fragmentAddi;
varying vec4 v_r_trans;
varying vec4 v_g_trans;
varying vec4 v_b_trans;
varying vec2 v_texcoord; 
uniform sampler2D texture0; 

// void main() 
// {   
// 	vec4 tmp = texture2D(texture0, v_texcoord); 
// 
// 	float s = 1.5;
// 
// 	vec3 test_v_g_trans = vec3(2, 0.3, 1); 
// 	//  vec3 test_v_g_trans = vec3(1, 1, 1); 
// 	float cmp_r = tmp.g - tmp.r * s; 
// 	float test_r = clamp(cmp_r, 0, 1) / cmp_r; 
// 	float cmp_b = tmp.g - tmp.b * s; 
// 	float test_b = clamp(cmp_b, 0, 1) / cmp_b; 
// 	vec3 trans = (test_v_g_trans - vec3(1, 1, 1)) * test_r * test_b + vec3(1, 1, 1); 
// 	tmp.xyz = vec3(tmp.r * trans.r, tmp.g * trans.g, tmp.b * trans.b); 
// 
// 	gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz; 
// 	gl_FragColor.w = tmp.w;   
// 	gl_FragColor *= v_fragmentColor.w; 
// 	gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w; 
// } 

void main() 
{   
	vec4 tmp = texture2D(texture0, v_texcoord); 

	float s = 1.5;
	vec3 test_v_g_trans = vec3(2, 0.3, 1); 

	float cmp_r = step(tmp.r * s, tmp.g); 
	float cmp_b = step(tmp.b * s, tmp.g); 
	vec3 trans = (test_v_g_trans - vec3(1, 1, 1)) * cmp_r * cmp_b + vec3(1, 1, 1); 
	tmp.xyz = vec3(tmp.r * trans.r, tmp.g * trans.g, tmp.b * trans.b); 

	gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz; 
	gl_FragColor.w = tmp.w;   
	gl_FragColor *= v_fragmentColor.w; 
	gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w; 
} 

);