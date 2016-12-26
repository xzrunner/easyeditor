// code from http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/

static const char* gaussian_blur_hori_header = STRINGIFY(

	float offset[3];
	float weight[3];
	
);

static const char* gaussian_blur_hori_body = STRINGIFY(

	offset[0] = 0.0;
	offset[1] = 1.3846153846;
	offset[2] = 3.2307692308;
	weight[0] = 0.2270270270;
	weight[1] = 0.3162162162;
	weight[2] = 0.0702702703;

	vec4 tmp = texture2D(u_texture0, v_texcoord);
	vec3 tc = tmp.rgb * weight[0];
	for (int i = 1; i < 3; i++) 
	{
		vec2 offset = vec2(offset[i], 0.0) / u_tex_width;
		tc += texture2D(u_texture0, v_texcoord + offset).rgb * weight[i];
		tc += texture2D(u_texture0, v_texcoord - offset).rgb * weight[i];
	}
	vec4 _DST_COL_ = vec4(tc, tmp.a);

);
