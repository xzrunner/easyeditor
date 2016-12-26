/**
 *  @remarks
 *    v_texcoord    varying vec2
 *    u_texture0     uniform sampler2D
 *    _DST_COL_     vec4
 */
static const char* shock_wave_body = STRINGIFY(

	vec2 texcoord = v_texcoord;
	float dis = distance(v_texcoord, u_center);
	if (dis <= (u_time + u_params.z) && dis >= (u_time - u_params.z))
	{
		float diff = dis - u_time;
		float pow_diff = 1.0 - pow(abs(diff * u_params.x), u_params.y);
		float diff_time = diff * pow_diff;
		vec2 diff_uv = normalize(v_texcoord - u_center);
		texcoord = v_texcoord + diff_uv * diff_time;
	}	
	vec4 _DST_COL_ = texture2D(u_texture0, texcoord);

);
