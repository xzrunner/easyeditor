static const char* NormalFragShader = STRINGIFY(

//precision highp float;

varying vec4 v_fragmentColor; 
varying vec4 v_fragmentAddi; 
varying vec2 v_texcoord;  
uniform sampler2D texture0;  

void main()  
{    
  vec4 tmp = texture2D(texture0, v_texcoord);  
  gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz;  
  gl_FragColor.w = tmp.w;    
  gl_FragColor *= v_fragmentColor.w;  
  gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w;  
}  

);
