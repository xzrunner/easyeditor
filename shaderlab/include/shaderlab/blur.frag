/**
 *  @remarks
 *    v_texcoord    varying vec2
 *    u_texture0     uniform sampler2D
 *    _DST_COL_     vec4
 */
static const char* blur_body = STRINGIFY(

	vec2 hori_texcoords[14];
	hori_texcoords[ 0] = v_texcoord + vec2(-0.028, 0.0) * u_radius;
    hori_texcoords[ 1] = v_texcoord + vec2(-0.024, 0.0) * u_radius;
    hori_texcoords[ 2] = v_texcoord + vec2(-0.020, 0.0) * u_radius;
    hori_texcoords[ 3] = v_texcoord + vec2(-0.016, 0.0) * u_radius;
    hori_texcoords[ 4] = v_texcoord + vec2(-0.012, 0.0) * u_radius;
    hori_texcoords[ 5] = v_texcoord + vec2(-0.008, 0.0) * u_radius;
    hori_texcoords[ 6] = v_texcoord + vec2(-0.004, 0.0) * u_radius;
    hori_texcoords[ 7] = v_texcoord + vec2( 0.004, 0.0) * u_radius;
    hori_texcoords[ 8] = v_texcoord + vec2( 0.008, 0.0) * u_radius;
    hori_texcoords[ 9] = v_texcoord + vec2( 0.012, 0.0) * u_radius;
    hori_texcoords[10] = v_texcoord + vec2( 0.016, 0.0) * u_radius;
    hori_texcoords[11] = v_texcoord + vec2( 0.020, 0.0) * u_radius;
    hori_texcoords[12] = v_texcoord + vec2( 0.024, 0.0) * u_radius;
    hori_texcoords[13] = v_texcoord + vec2( 0.028, 0.0) * u_radius;	

    vec2 vert_texcoords[14];
    vert_texcoords[ 0] = v_texcoord + vec2(0.0, -0.028) * u_radius;
    vert_texcoords[ 1] = v_texcoord + vec2(0.0, -0.024) * u_radius;
    vert_texcoords[ 2] = v_texcoord + vec2(0.0, -0.020) * u_radius;
    vert_texcoords[ 3] = v_texcoord + vec2(0.0, -0.016) * u_radius;
    vert_texcoords[ 4] = v_texcoord + vec2(0.0, -0.012) * u_radius;
    vert_texcoords[ 5] = v_texcoord + vec2(0.0, -0.008) * u_radius;
    vert_texcoords[ 6] = v_texcoord + vec2(0.0, -0.004) * u_radius;
    vert_texcoords[ 7] = v_texcoord + vec2(0.0,  0.004) * u_radius;
    vert_texcoords[ 8] = v_texcoord + vec2(0.0,  0.008) * u_radius;
    vert_texcoords[ 9] = v_texcoord + vec2(0.0,  0.012) * u_radius;
    vert_texcoords[10] = v_texcoord + vec2(0.0,  0.016) * u_radius;
    vert_texcoords[11] = v_texcoord + vec2(0.0,  0.020) * u_radius;
    vert_texcoords[12] = v_texcoord + vec2(0.0,  0.024) * u_radius;
    vert_texcoords[13] = v_texcoord + vec2(0.0,  0.028) * u_radius;
    
 	vec4 _DST_COL_;

    _DST_COL_ += texture2D(u_texture0, hori_texcoords[ 0])*0.0044299121055113265;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[ 1])*0.00895781211794;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[ 2])*0.0215963866053;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[ 3])*0.0443683338718;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[ 4])*0.0776744219933;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[ 5])*0.115876621105;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[ 6])*0.147308056121;
    _DST_COL_ += texture2D(u_texture0, v_texcoord         )*0.159576912161;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[ 7])*0.147308056121;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[ 8])*0.115876621105;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[ 9])*0.0776744219933;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[10])*0.0443683338718;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[11])*0.0215963866053;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[12])*0.00895781211794;
    _DST_COL_ += texture2D(u_texture0, hori_texcoords[13])*0.0044299121055113265;

    _DST_COL_ += texture2D(u_texture0, vert_texcoords[ 0])*0.0044299121055113265;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[ 1])*0.00895781211794;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[ 2])*0.0215963866053;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[ 3])*0.0443683338718;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[ 4])*0.0776744219933;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[ 5])*0.115876621105;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[ 6])*0.147308056121;
    _DST_COL_ += texture2D(u_texture0, v_texcoord         )*0.159576912161;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[ 7])*0.147308056121;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[ 8])*0.115876621105;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[ 9])*0.0776744219933;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[10])*0.0443683338718;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[11])*0.0215963866053;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[12])*0.00895781211794;
    _DST_COL_ += texture2D(u_texture0, vert_texcoords[13])*0.0044299121055113265;

);
