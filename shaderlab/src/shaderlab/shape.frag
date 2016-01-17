static const char* shape_frag = STRINGIFY(

varying vec4 v_color;

void main()
{  
	gl_FragColor = v_color;
}

);
