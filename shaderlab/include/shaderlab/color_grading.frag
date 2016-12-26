

static const char* color_grading_header = STRINGIFY(

\n#define INV16 (1.0 / 16.0) \n
\n#define INV18 (1.0 / 18.0) \n
\n#define INV288 (1.0 / 288.0) \n

);

/**
 *  @remarks
 *    v_texcoord    varying vec2
 *    u_texture0    uniform sampler2D
 *    _DST_COL_     vec4
 */
static const char* color_grading_body = STRINGIFY(
	vec4 src_col = texture2D(u_texture0, v_texcoord);
	vec3 color = src_col.xyz;
	
	color.r *= INV16;
	color.b *= 15.0;
	
	color.r = INV288 + (1.0 - INV18 * 2.0) * color.r;
	color.g = INV18 + (1.0 - INV18 * 2.0) * color.g;
		     
	float shift = floor(color.b);
	float start = shift * INV16;
	color.rgb = mix(texture2D(u_col_lut, color.rg + vec2(start, 0.0)).rgb, texture2D(u_col_lut, color.rg + vec2(start + INV16, 0.0)).rgb, color.b - shift);
	
	vec4 _DST_COL_ = vec4(color, src_col.a);
);
