static const char* ShadowFragShader = STRINGIFY(

varying vec4 v_fragmentColor;

void main()
{  
	gl_FragColor = gl_Color;
	
	if (gl_Color.r == gl_Color.g && gl_Color.g == gl_Color.b) {
		gl_FragColor.a = gl_Color.a * gl_Color.a * gl_Color.a;
	}
}

);
