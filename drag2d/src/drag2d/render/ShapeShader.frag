static const char* ShapeFragShader = STRINGIFY(

varying vec4 v_fragmentColor;

void main()
{  
	gl_FragColor = gl_Color;
}

);
