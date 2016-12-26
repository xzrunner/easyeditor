// code from http://www.geeks3d.com/20110428/shader-library-swirl-post-processing-filter-in-glsl/

/**
 *  @remarks
 *    v_texcoord    varying vec2
 *    u_texture0     uniform sampler2D
 *    _DST_COL_     vec4
 */
static const char* swirl_body = STRINGIFY(

	vec2 tex_size = textureSize(u_texture0, 0);
	vec2 tc = v_texcoord * tex_size;
	tc -= u_center;
	float dist = length(tc);
	if (dist < u_radius)
	{
		float percent = (u_radius - dist) / u_radius;
		float theta = percent * percent * u_angle * 8.0;
		float s = sin(theta);
		float c = cos(theta);
		tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));		
	}
	tc += u_center;
	vec4 _DST_COL_ = texture2D(u_texture0, tc / tex_size);

);
