static const char* FontFragShader = STRINGIFY(

varying vec4 v_fragmentColor;
varying vec2 v_texcoord; 
uniform sampler2D texture0; 

void main() 
{   
	float mask = texture2D(texture0, v_texcoord).a;   
	vec4 clr;                                         
	clr.rgb = vec3(1.0, 0.0, 0.0);                    
	if (mask < 0.5)				                     
	clr.a = 0.0;                                    
	else                                              
	clr.a = 1.0;                                    
	gl_FragColor = clr;                               
} 

);