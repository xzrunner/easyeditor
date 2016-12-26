/**
 *  @remarks
 *    v_texcoord    varying vec2
 *    u_texture0     uniform sampler2D
 *    _DST_COL_     vec4
 */
static const char* edge_detect_body = STRINGIFY(
	ivec2 ires = textureSize(u_texture0, 0);
	float ResS = float(ires.s);
	float ResT = float(ires.t);
	vec3 irgb = texture2D(u_texture0, v_texcoord).rgb;

	vec2 stp0 = vec2(1.0/ResS, 0.0);
	vec2 st0p = vec2(0.0, 1.0/ResT);
	vec2 stpp = vec2(1.0/ResS, 1.0/ResT);
	vec2 stpm = vec2(1.0/ResS, -1.0/ResT);

	const vec3 w = vec3(0.2125, 0.7154, 0.0721);
	float i00 	= dot(texture2D(u_texture0, v_texcoord).rgb, w);
	float im1m1 = dot(texture2D(u_texture0, v_texcoord - stpp).rgb, w);
	float ip1p1 = dot(texture2D(u_texture0, v_texcoord + stpp).rgb, w);
	float im1p1 = dot(texture2D(u_texture0, v_texcoord - stpm).rgb, w);
	float ip1m1 = dot(texture2D(u_texture0, v_texcoord + stpm).rgb, w);
	float im10  = dot(texture2D(u_texture0, v_texcoord - stp0).rgb, w);
	float ip10  = dot(texture2D(u_texture0, v_texcoord + stp0).rgb, w);
	float i0m1  = dot(texture2D(u_texture0, v_texcoord - st0p).rgb, w);
	float i0p1  = dot(texture2D(u_texture0, v_texcoord + st0p).rgb, w);

	float h = -1.0 * im1p1 - 2.0 * i0p1 - 1.0 * ip1p1 + 1.0 * im1m1 + 2.0 * i0m1 + 1.0 * ip1m1;
	float v = -1.0 * im1m1 - 2.0 * im10 - 1.0 * im1p1 + 1.0 * ip1m1 + 2.0 * ip10 + 1.0 * ip1p1;
	float mag = length(vec2(h, v));
	vec3 target = vec3(mag, 0, 0);
	vec4 _DST_COL_ = vec4(mix(irgb, target, u_blend), texture2D(u_texture0, v_texcoord).a);
//	vec4 _DST_COL_ = vec4(mix(irgb, target, 0.0), 1.0);

);
