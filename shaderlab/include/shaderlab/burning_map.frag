static const char* burning_map_header = STRINGIFY(

\n#define Blend(base, blend, funcf) 		vec3(funcf(base.r, blend.r), funcf(base.g, blend.g), funcf(base.b, blend.b)) \n

\n#define BlendLinearDodge(base, blend)		BlendAdd(base, blend) \n
\n#define BlendAdd(base, blend) 			min(base + blend, vec3(1.0, 1.0, 1.0)) \n

\n#define BlendLighten(base, blend)			Blend(base, blend, BlendLightenf) \n
\n#define BlendLightenf(base, blend) 		max(blend, base) \n

);

/**
 *  @remarks
 *    v_texcoord			varying vec2
 *    u_texture0			uniform sampler2D
 *    u_texture1			uniform sampler2D 
 *    u_height_map_tex		uniform sampler2D  
 *    u_border_gradient_tex	uniform sampler2D  
 *	  u_lifetime			float  
 *	  u_time				float
 *    _DST_COL_				vec4
 */
static const char* burning_map_body = STRINGIFY(

	vec4 _bm_lower_ = texture2D(u_texture0, v_texcoord);
	vec4 _bm_upper_ = texture2D(u_texture1, v_texcoord);
	vec4 _bm_height_ = texture2D(u_height_map_tex, v_texcoord);
	
	float _bm_blend_ = _bm_height_.x;

	float _bm_edge_ = 1.0 - min(1.0, u_time / u_lifetime);

	vec4 _DST_COL_;
	if (_bm_blend_ > _bm_edge_) {
		_DST_COL_ = _bm_lower_;
	} else {
		if (_bm_edge_ - _bm_blend_ > 0.25) {
			_DST_COL_ = _bm_upper_;
		} else {
			float x = (_bm_edge_ - _bm_blend_) / 0.25;
			vec4 _bm_border_ = texture2D(u_border_gradient_tex, vec2(x, 0.5));

			const float bound1 = 90.0;
			if (x < bound1 / 128.0) {
				vec3 blend1 = BlendLinearDodge(_bm_lower_.rgb, _bm_border_.rgb);
				vec3 blend2 = BlendLighten(_bm_lower_.rgb, _bm_border_.rgb);
				float factor = x * 128.0 / bound1;
				_DST_COL_ = vec4(blend1, 1.0) * (1.0 - factor) + vec4(blend2, 1.0) * factor;
			} else {
				_DST_COL_ = _bm_upper_ * (1.0 - _bm_border_.a) + _bm_border_ * (_bm_border_.a);
			}
		}
	}

);