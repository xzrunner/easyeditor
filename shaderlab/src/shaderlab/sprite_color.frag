static const char* sprite_color_frag = STRINGIFY(

varying vec4 v_color;
varying vec4 v_additive;
varying vec2 v_texcoord; 

uniform sampler2D texture0; 

void main()
{  
	vec4 tmp = texture2D(texture0, v_texcoord); 
	gl_FragColor.xyz = tmp.xyz * v_color.xyz; 
	gl_FragColor.w = tmp.w;   
	gl_FragColor *= v_color.w; 
	gl_FragColor.xyz += v_additive.xyz * tmp.w * v_color.w; 
}

);
