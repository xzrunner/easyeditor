/**
 *  @remarks
 *    v_texcoord    varying vec2
 *    u_texture0     uniform sampler2D
 *    _DST_COL_     vec4
 */
static const char* relief_body = STRINGIFY(

	ivec2 ires = textureSize(u_texture0, 0);
	float ResS = float(ires.s);
	float ResT = float(ires.t);
	vec4 rgba = texture2D(u_texture0, v_texcoord);

	vec2 stp0 = vec2(1.0 / ResS, 0.0);
	vec2 stpp = vec2(1.0 / ResS, 1.0 / ResT);
	vec3 c00 = rgba.rgb;
	vec3 cp1p1 = texture2D(u_texture0, v_texcoord + stpp).rgb;

	vec3 diffs = c00 - cp1p1;
	float max = diffs.r;
	if (abs(diffs.g) > abs(max)) max = diffs.g;
	if (abs(diffs.b) > abs(max)) max = diffs.b;

	float gray = clamp(max + 0.5, 0.0, 1.0);
	vec3 color = vec3(gray, gray, gray);
	vec4 _DST_COL_ = vec4(color * rgba.a, rgba.a);

);
