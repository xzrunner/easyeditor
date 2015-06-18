static const char* ReliefFragShader = STRINGIFY(

//precision highp float;

varying vec2 v_texcoord;  
uniform sampler2D texture0;  

void main()  
{    
	ivec2 ires = textureSize(texture0, 0);
	float ResS = float(ires.s);
	float ResT = float(ires.t);
	vec3 irgb = texture2D(texture0, v_texcoord).rgb;

	vec2 stp0 = vec2(1.0 / ResS, 0.0);
	vec2 stpp = vec2(1.0 / ResS, 1.0 / ResT);
	vec3 c00 = irgb;
	vec3 cp1p1 = texture2D(texture0, v_texcoord + stpp).rgb;

	vec3 diffs = c00 - cp1p1;
	float max = diffs.r;
	if (abs(diffs.g) > abs(max)) max = diffs.g;
	if (abs(diffs.b) > abs(max)) max = diffs.b;

	float gray = clamp(max + 0.5, 0.0, 1.0);
	vec3 color = vec3(gray, gray, gray);
	gl_FragColor = vec4(color, 1.0);
}  

);
