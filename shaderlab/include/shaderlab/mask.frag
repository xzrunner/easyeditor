static const char* mask_body = STRINGIFY(

	vec4 mask = texture2D(u_texture1, v_texcoord_mask);
	vec4 _DST_COL_ = _SRC_COL_ * mask.a;

);