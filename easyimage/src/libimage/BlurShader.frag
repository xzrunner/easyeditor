static const char* BlurFragShader = STRINGIFY(

//precision highp float;

varying vec4 v_fragmentColor; 
varying vec4 v_fragmentAddi; 
varying vec2 v_texcoord;  
uniform sampler2D texture0;  

uniform float u_radius;

void main()  
{
	vec2 texcoords[2][14];
	texcoords[0][ 0] = v_texcoord + vec2(-0.028, 0.0);
    texcoords[0][ 1] = v_texcoord + vec2(-0.024, 0.0);
    texcoords[0][ 2] = v_texcoord + vec2(-0.020, 0.0);
    texcoords[0][ 3] = v_texcoord + vec2(-0.016, 0.0);
    texcoords[0][ 4] = v_texcoord + vec2(-0.012, 0.0);
    texcoords[0][ 5] = v_texcoord + vec2(-0.008, 0.0);
    texcoords[0][ 6] = v_texcoord + vec2(-0.004, 0.0);
    texcoords[0][ 7] = v_texcoord + vec2( 0.004, 0.0);
    texcoords[0][ 8] = v_texcoord + vec2( 0.008, 0.0);
    texcoords[0][ 9] = v_texcoord + vec2( 0.012, 0.0);
    texcoords[0][10] = v_texcoord + vec2( 0.016, 0.0);
    texcoords[0][11] = v_texcoord + vec2( 0.020, 0.0);
    texcoords[0][12] = v_texcoord + vec2( 0.024, 0.0);
    texcoords[0][13] = v_texcoord + vec2( 0.028, 0.0);	

    texcoords[1][ 0] = v_texcoord + vec2(0.0, -0.028);
    texcoords[1][ 1] = v_texcoord + vec2(0.0, -0.024);
    texcoords[1][ 2] = v_texcoord + vec2(0.0, -0.020);
    texcoords[1][ 3] = v_texcoord + vec2(0.0, -0.016);
    texcoords[1][ 4] = v_texcoord + vec2(0.0, -0.012);
    texcoords[1][ 5] = v_texcoord + vec2(0.0, -0.008);
    texcoords[1][ 6] = v_texcoord + vec2(0.0, -0.004);
    texcoords[1][ 7] = v_texcoord + vec2(0.0,  0.004);
    texcoords[1][ 8] = v_texcoord + vec2(0.0,  0.008);
    texcoords[1][ 9] = v_texcoord + vec2(0.0,  0.012);
    texcoords[1][10] = v_texcoord + vec2(0.0,  0.016);
    texcoords[1][11] = v_texcoord + vec2(0.0,  0.020);
    texcoords[1][12] = v_texcoord + vec2(0.0,  0.024);
    texcoords[1][13] = v_texcoord + vec2(0.0,  0.028);
    
    for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 14; ++j) {
			texcoords[i][j] = (texcoords[i][j] - v_texcoord) * u_radius + v_texcoord;
		}
    }

 	vec4 tmp;

    tmp += texture2D(texture0, texcoords[0][ 0])*0.0044299121055113265;
    tmp += texture2D(texture0, texcoords[0][ 1])*0.00895781211794;
    tmp += texture2D(texture0, texcoords[0][ 2])*0.0215963866053;
    tmp += texture2D(texture0, texcoords[0][ 3])*0.0443683338718;
    tmp += texture2D(texture0, texcoords[0][ 4])*0.0776744219933;
    tmp += texture2D(texture0, texcoords[0][ 5])*0.115876621105;
    tmp += texture2D(texture0, texcoords[0][ 6])*0.147308056121;
    tmp += texture2D(texture0, v_texcoord         )*0.159576912161;
    tmp += texture2D(texture0, texcoords[0][ 7])*0.147308056121;
    tmp += texture2D(texture0, texcoords[0][ 8])*0.115876621105;
    tmp += texture2D(texture0, texcoords[0][ 9])*0.0776744219933;
    tmp += texture2D(texture0, texcoords[0][10])*0.0443683338718;
    tmp += texture2D(texture0, texcoords[0][11])*0.0215963866053;
    tmp += texture2D(texture0, texcoords[0][12])*0.00895781211794;
    tmp += texture2D(texture0, texcoords[0][13])*0.0044299121055113265;

    tmp += texture2D(texture0, texcoords[1][ 0])*0.0044299121055113265;
    tmp += texture2D(texture0, texcoords[1][ 1])*0.00895781211794;
    tmp += texture2D(texture0, texcoords[1][ 2])*0.0215963866053;
    tmp += texture2D(texture0, texcoords[1][ 3])*0.0443683338718;
    tmp += texture2D(texture0, texcoords[1][ 4])*0.0776744219933;
    tmp += texture2D(texture0, texcoords[1][ 5])*0.115876621105;
    tmp += texture2D(texture0, texcoords[1][ 6])*0.147308056121;
    tmp += texture2D(texture0, v_texcoord         )*0.159576912161;
    tmp += texture2D(texture0, texcoords[1][ 7])*0.147308056121;
    tmp += texture2D(texture0, texcoords[1][ 8])*0.115876621105;
    tmp += texture2D(texture0, texcoords[1][ 9])*0.0776744219933;
    tmp += texture2D(texture0, texcoords[1][10])*0.0443683338718;
    tmp += texture2D(texture0, texcoords[1][11])*0.0215963866053;
    tmp += texture2D(texture0, texcoords[1][12])*0.00895781211794;
    tmp += texture2D(texture0, texcoords[1][13])*0.0044299121055113265;

    tmp *= 0.5;

	gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz;  
	gl_FragColor.w = tmp.w;    
	gl_FragColor *= v_fragmentColor.w;  
	gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w;  
}

);
