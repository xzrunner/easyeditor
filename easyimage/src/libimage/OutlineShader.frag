static const char* OutlineFragShader = STRINGIFY(

//precision highp float;

varying vec2 v_texcoord;  
uniform sampler2D texture0;  

uniform float u_blend;

void main()  
{    
	ivec2 ires = textureSize(texture0, 0);
	float ResS = float(ires.s);
	float ResT = float(ires.t);
	vec3 irgb = texture2D(texture0, v_texcoord).rgb;
	
	vec2 stp0 = vec2(1.0/ResS, 0.0);
	vec2 st0p = vec2(0.0, 1.0/ResT);
	vec2 stpp = vec2(1.0/ResS, 1.0/ResT);
	vec2 stpm = vec2(1.0/ResS, -1.0/ResT);
	
	float w_multi = texture2D(texture0, v_texcoord).w *
					texture2D(texture0, v_texcoord - stp0).w *
					texture2D(texture0, v_texcoord + stp0).w *
					texture2D(texture0, v_texcoord - st0p).w *
					texture2D(texture0, v_texcoord + st0p).w *
					texture2D(texture0, v_texcoord - stpp).w *
					texture2D(texture0, v_texcoord + stpp).w *
					texture2D(texture0, v_texcoord - stpm).w *
					texture2D(texture0, v_texcoord + stpm).w;
	float w_add   = texture2D(texture0, v_texcoord).w +
					texture2D(texture0, v_texcoord - stp0).w +
					texture2D(texture0, v_texcoord + stp0).w +
					texture2D(texture0, v_texcoord - st0p).w +
					texture2D(texture0, v_texcoord + st0p).w +
					texture2D(texture0, v_texcoord - stpp).w +
					texture2D(texture0, v_texcoord + stpp).w +
					texture2D(texture0, v_texcoord - stpm).w +
					texture2D(texture0, v_texcoord + stpm).w;

	float red = 1.0f * (1 - sign(w_multi)) * sign(w_add);
		
	gl_FragColor = vec4(red, 0, 0, red);
}

);
