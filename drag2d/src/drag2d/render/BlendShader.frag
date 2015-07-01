// copy from https://gist.github.com/jashmenn/5328940

static const char* BlendFragShader = STRINGIFY(

/*
** Float blending modes
*/
#define BlendLinearDodgef 				BlendAddf \n
#define BlendLinearBurnf 				BlendSubstractf \n
#define BlendAddf(base, blend) 			min(base + blend, 1.0) \n
#define BlendSubstractf(base, blend) 	max(base + blend - 1.0, 0.0) \n
#define BlendLightenf(base, blend) 		max(blend, base) \n
#define BlendDarkenf(base, blend) 		min(blend, base) \n
#define BlendLinearLightf(base, blend) 	(blend < 0.5 ? BlendLinearBurnf(base, (2.0 * blend)) : BlendLinearDodgef(base, (2.0 * (blend - 0.5)))) \n
#define BlendScreenf(base, blend) 		(1.0 - ((1.0 - base) * (1.0 - blend))) \n
#define BlendOverlayf(base, blend) 		(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend))) \n
#define BlendSoftLightf(base, blend) 	((blend < 0.5) ? (2.0 * base * blend + base * base * (1.0 - 2.0 * blend)) : (sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend))) \n
#define BlendColorDodgef(base, blend) 	((blend == 1.0) ? blend : min(base / (1.0 - blend), 1.0)) \n
#define BlendColorBurnf(base, blend) 	((blend == 0.0) ? blend : max((1.0 - ((1.0 - base) / blend)), 0.0)) \n
#define BlendVividLightf(base, blend) 	((blend < 0.5) ? BlendColorBurnf(base, (2.0 * blend)) : BlendColorDodgef(base, (2.0 * (blend - 0.5)))) \n
#define BlendPinLightf(base, blend) 	((blend < 0.5) ? BlendDarkenf(base, (2.0 * blend)) : BlendLightenf(base, (2.0 *(blend - 0.5)))) \n
#define BlendHardMixf(base, blend) 		((BlendVividLightf(base, blend) < 0.5) ? 0.0 : 1.0) \n
#define BlendReflectf(base, blend) 		((blend == 1.0) ? blend : min(base * base / (1.0 - blend), 1.0)) \n

/*
** Vector3 blending modes
*/

// Component wise blending
#define Blend(base, blend, funcf) 		vec3(funcf(base.r, blend.r), funcf(base.g, blend.g), funcf(base.b, blend.b)) \n
 
#define BlendNormal(base, blend) 		(blend) \n
#define BlendLighten					BlendLightenf \n
#define BlendDarken						BlendDarkenf \n
#define BlendMultiply(base, blend) 		(base * blend) \n
#define BlendAverage(base, blend) 		((base + blend) / 2.0) \n
#define BlendAdd(base, blend) 			min(base + blend, vec3(1.0)) \n
#define BlendSubstract(base, blend) 	max(base + blend - vec3(1.0), vec3(0.0)) \n
#define BlendDifference(base, blend) 	abs(base - blend) \n
#define BlendNegation(base, blend) 		(vec3(1.0) - abs(vec3(1.0) - base - blend)) \n
#define BlendExclusion(base, blend) 	(base + blend - 2.0 * base * blend) \n
#define BlendScreen(base, blend) 		Blend(base, blend, BlendScreenf) \n
#define BlendOverlay(base, blend) 		Blend(base, blend, BlendOverlayf) \n
#define BlendSoftLight(base, blend) 	Blend(base, blend, BlendSoftLightf) \n
#define BlendHardLight(base, blend) 	BlendOverlay(blend, base) \n
#define BlendColorDodge(base, blend) 	Blend(base, blend, BlendColorDodgef) \n
#define BlendColorBurn(base, blend) 	Blend(base, blend, BlendColorBurnf) \n
#define BlendLinearDodge				BlendAdd \n
#define BlendLinearBurn					BlendSubstract \n
// Linear Light is another contrast-increasing mode
// If the blend color is darker than midgray, Linear Light darkens the image by decreasing the brightness. If the blend color is lighter than midgray, the result is a brighter image due to increased brightness.
#define BlendLinearLight(base, blend) 	Blend(base, blend, BlendLinearLightf) \n
#define BlendVividLight(base, blend) 	Blend(base, blend, BlendVividLightf) \n
#define BlendPinLight(base, blend) 		Blend(base, blend, BlendPinLightf) \n
#define BlendHardMix(base, blend) 		Blend(base, blend, BlendHardMixf) \n
#define BlendReflect(base, blend) 		Blend(base, blend, BlendReflectf) \n
#define BlendGlow(base, blend) 			BlendReflect(blend, base) \n
#define BlendPhoenix(base, blend) 		(min(base, blend) - max(base, blend) + vec3(1.0)) \n
#define BlendOpacity(base, blend, F, O) (F(base, blend) * O + blend * (1.0 - O)) \n

uniform int u_mode;

uniform sampler2D texture0;
uniform sampler2D texture1;

varying vec4 v_fragmentColor;
varying vec4 v_fragmentAddi;
varying vec2 v_texcoord;
varying vec2 v_texcoord_base;

void main()
{
	vec4 base = texture2D(texture1, v_texcoord_base);
	vec4 blend = texture2D(texture0, v_texcoord);
		
	vec3 result;
	// normal
	if (u_mode == 0) {
		result = BlendNormal(base.rgb, blend.rgb);
	} 
	// darken modes
	else if (u_mode == 10) {
		result = BlendDarken(base.rgb, blend.rgb);		
	} else if (u_mode == 11) {
		result = BlendMultiply(base.rgb, blend.rgb);
	} else if (u_mode == 12) {
		result = BlendColorBurn(base.rgb, blend.rgb);
	} else if (u_mode == 13) {
		result = BlendLinearBurn(base.rgb, blend.rgb);
	}
	// lighten modes
	else if (u_mode == 20) {
		result = BlendLighten(base.rgb, blend.rgb);
	} else if (u_mode == 21) {
		result = BlendScreen(base.rgb, blend.rgb);
	} else if (u_mode == 22) {
		result = BlendColorDodge(base.rgb, blend.rgb);
	} else if (u_mode == 23) {
		result = BlendLinearDodge(base.rgb, blend.rgb);
	} else if (u_mode == 24) {
		result = BlendLinearDodge(base.rgb, blend.rgb);
	}
	// saturation modes
	else if (u_mode == 30) {
		result = BlendOverlay(base.rgb, blend.rgb);
	} else if (u_mode == 31) {
		result = BlendSoftLight(base.rgb, blend.rgb);
	} else if (u_mode == 32) {
		result = BlendHardLight(base.rgb, blend.rgb);
	} else if (u_mode == 33) {
		result = BlendVividLight(base.rgb, blend.rgb);
	} else if (u_mode == 34) {
		result = BlendLinearLight(base.rgb, blend.rgb);
	} else if (u_mode == 35) {
		result = BlendPinLight(base.rgb, blend.rgb);
	} else if (u_mode == 36) {
		result = BlendHardMix(base.rgb, blend.rgb);
	}
	// substraction modes
	else if (u_mode == 40) {
		result = BlendDifference(base.rgb, blend.rgb);
	} else if (u_mode == 41) {
		result = BlendExclusion(base.rgb, blend.rgb);
	} else if (u_mode == 42) {
		result = BlendExclusion(base.rgb, blend.rgb);
	}
	// color modes 
	
	// others
	else {
		result = BlendNormal(base.rgb, blend.rgb);
	}
	
// 	if (blend.a <= 0.0) {
// 		gl_FragColor = vec4(0.0,0.0,0.0,0.0);
// 	} else {
// 		gl_FragColor = vec4(result, 1.0);
// 	}

// 	gl_FragColor.xyz = result * v_fragmentColor.xyz; 
// 	gl_FragColor.w = blend.w;   
// 	gl_FragColor *= v_fragmentColor.w; 
// 	gl_FragColor.xyz += v_fragmentAddi.xyz * blend.w; 
	
	if (blend.a <= 0.0) {
		gl_FragColor = vec4(0.0,0.0,0.0,0.0);
	} else {
		gl_FragColor.xyz = result * v_fragmentColor.xyz * blend.a; 
		gl_FragColor.w = blend.w;   
		gl_FragColor *= v_fragmentColor.w; 
		gl_FragColor.xyz += v_fragmentAddi.xyz * blend.w; 
	
// 		result.rgb = result.rgb * blend.a;			
// 		gl_FragColor = vec4(result, blend.a);
	}	
}

);