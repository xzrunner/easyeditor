
static const char* blend_header = STRINGIFY(

/*
** Float blending modes
*/
\n#define BlendLinearDodgef(base, blend) 	BlendAddf(base, blend) \n
\n#define BlendLinearBurnf(base, blend) 	BlendSubstractf(base, blend) \n
\n#define BlendAddf(base, blend) 			min(base + blend, 1.0) \n
\n#define BlendSubstractf(base, blend) 		max(base + blend - 1.0, 0.0) \n
\n#define BlendLightenf(base, blend) 		max(blend, base) \n
\n#define BlendDarkenf(base, blend) 		min(blend, base) \n
\n#define BlendLinearLightf(base, blend) 	(blend < 0.5 ? BlendLinearBurnf(base, (2.0 * blend)) : BlendLinearDodgef(base, (2.0 * (blend - 0.5)))) \n
\n#define BlendScreenf(base, blend) 		(1.0 - ((1.0 - base) * (1.0 - blend))) \n
\n#define BlendOverlayf(base, blend) 		(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend))) \n
\n#define BlendSoftLightf(base, blend) 		((blend < 0.5) ? (2.0 * base * blend + base * base * (1.0 - 2.0 * blend)) : (sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend))) \n
\n#define BlendColorDodgef(base, blend) 	((blend == 1.0) ? blend : min(base / (1.0 - blend), 1.0)) \n
\n#define BlendColorBurnf(base, blend) 		((blend == 0.0) ? blend : max((1.0 - ((1.0 - base) / blend)), 0.0)) \n
\n#define BlendVividLightf(base, blend) 	((blend < 0.5) ? BlendColorBurnf(base, (2.0 * blend)) : BlendColorDodgef(base, (2.0 * (blend - 0.5)))) \n
\n#define BlendPinLightf(base, blend) 		((blend < 0.5) ? BlendDarkenf(base, (2.0 * blend)) : BlendLightenf(base, (2.0 *(blend - 0.5)))) \n
\n#define BlendHardMixf(base, blend) 		((BlendVividLightf(base, blend) < 0.5) ? 0.0 : 1.0) \n
\n#define BlendReflectf(base, blend) 		((blend == 1.0) ? blend : min(base * base / (1.0 - blend), 1.0)) \n

/*
** Vector3 blending modes
*/

// Component wise blending
\n#define Blend(base, blend, funcf) 		vec3(funcf(base.r, blend.r), funcf(base.g, blend.g), funcf(base.b, blend.b)) \n
 
\n#define BlendNormal(base, blend) 			(blend) \n
\n#define BlendLighten(base, blend)			Blend(base, blend, BlendLightenf) \n
\n#define BlendDarken(base, blend)			Blend(base, blend, BlendDarkenf) \n
\n#define BlendMultiply(base, blend) 		(base * blend) \n
\n#define BlendAverage(base, blend) 		((base + blend) / 2.0) \n
\n#define BlendAdd(base, blend) 			min(base + blend, vec3(1.0, 1.0, 1.0)) \n
\n#define BlendSubstract(base, blend) 		Blend(base, blend, BlendSubstractf) \n
\n#define BlendDifference(base, blend) 		abs(base - blend) \n
\n#define BlendNegation(base, blend) 		(vec3(1.0, 1.0, 1.0) - abs(vec3(1.0, 1.0, 1.0) - base - blend)) \n
\n#define BlendExclusion(base, blend) 		(base + blend - 2.0 * base * blend) \n
\n#define BlendScreen(base, blend) 			Blend(base, blend, BlendScreenf) \n
\n#define BlendOverlay(base, blend) 		Blend(base, blend, BlendOverlayf) \n
\n#define BlendSoftLight(base, blend) 		Blend(base, blend, BlendSoftLightf) \n
\n#define BlendHardLight(base, blend) 		BlendOverlay(blend, base) \n
\n#define BlendColorDodge(base, blend) 		Blend(base, blend, BlendColorDodgef) \n
\n#define BlendColorBurn(base, blend) 		Blend(base, blend, BlendColorBurnf) \n
\n#define BlendLinearDodge(base, blend)		Blend(base, blend, BlendAddf) \n
\n#define BlendLinearBurn(base, blend)		Blend(base, blend, BlendLinearBurnf) \n
// Linear Light is another contrast-increasing mode
// If the blend color is darker than midgray, Linear Light darkens the image by decreasing the brightness. If the blend color is lighter than midgray, the result is a brighter image due to increased brightness.
\n#define BlendLinearLight(base, blend) 	Blend(base, blend, BlendLinearLightf) \n
\n#define BlendVividLight(base, blend) 		Blend(base, blend, BlendVividLightf) \n
\n#define BlendPinLight(base, blend) 		Blend(base, blend, BlendPinLightf) \n
\n#define BlendHardMix(base, blend) 		Blend(base, blend, BlendHardMixf) \n
\n#define BlendReflect(base, blend) 		Blend(base, blend, BlendReflectf) \n
\n#define BlendGlow(base, blend) 			BlendReflect(blend, base) \n
\n#define BlendPhoenix(base, blend) 		(min(base, blend) - max(base, blend) + vec3(1.0, 1.0, 1.0)) \n
\n#define BlendOpacity(base, blend, F, O)	(F(base, blend) * O + blend * (1.0 - O)) \n

);

static const char* blend_body = STRINGIFY(

	vec4 base = texture2D(u_texture1, v_texcoord_base);
	base.rgb = base.rgb / base.a;
	
	_SRC_COL_.rgb = _SRC_COL_.rgb / _SRC_COL_.a;

	vec3 _blend_;
	// normal
	if (u_mode == 0) {
		_blend_ = BlendNormal(base.rgb, _SRC_COL_.rgb);
	}
	// darken modes
	else if (u_mode == 10) {
		_blend_ = BlendDarken(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 11) {
		_blend_ = BlendMultiply(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 12) {
		_blend_ = BlendColorBurn(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 13) {
		_blend_ = BlendLinearBurn(base.rgb, _SRC_COL_.rgb);
	}
	// lighten modes
	else if (u_mode == 20) {
		_blend_ = BlendLighten(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 21) {
		_blend_ = BlendScreen(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 22) {
		_blend_ = BlendColorDodge(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 23) {
		_blend_ = BlendLinearDodge(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 24) {
		_blend_ = BlendLinearDodge(base.rgb, _SRC_COL_.rgb);
	}
	// saturation modes
	else if (u_mode == 30) {
		_blend_ = BlendOverlay(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 31) {
		_blend_ = BlendSoftLight(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 32) {
		_blend_ = BlendHardLight(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 33) {
		// todo: for "Too many vertex shader constants"
		_blend_ = Blend(base.rgb, _SRC_COL_.rgb, BlendVividLightf);
	} else if (u_mode == 34) {
		// todo: for "Too many vertex shader constants"
		_blend_ = Blend(base.rgb, _SRC_COL_.rgb, BlendLinearLightf);
	} else if (u_mode == 35) {
		_blend_ = BlendPinLight(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 36) {
		// fixed for shader link err on sumsung note3
//		_blend_ = BlendHardMix(base.rgb, _SRC_COL_.rgb);
	}
	// substraction modes
	else if (u_mode == 40) {
		_blend_ = BlendDifference(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 41) {
		_blend_ = BlendExclusion(base.rgb, _SRC_COL_.rgb);
	} else if (u_mode == 42) {
		_blend_ = BlendExclusion(base.rgb, _SRC_COL_.rgb);
	}
	// color modes 
	
	// others
	else {
		_blend_ = BlendNormal(base.rgb, _SRC_COL_.rgb);
	}
	
	vec4 _DST_COL_ = vec4(_blend_ * _SRC_COL_.w, _SRC_COL_.w);

);
