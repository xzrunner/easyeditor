static const char* LanczosFragShader = STRINGIFY(

precision highp float;

uniform sampler2D texture0;

varying vec2 v_center_texcoord;
varying vec2 v_one_step_left_texcoord;
varying vec2 v_two_step_left_texcoord;
varying vec2 v_three_step_left_texcoord;
varying vec2 v_four_step_left_texcoord;
varying vec2 v_one_step_right_texcoord;
varying vec2 v_two_step_right_texcoord;
varying vec2 v_three_step_right_texcoord;
varying vec2 v_four_step_right_texcoord;

// sinc(x) * sinc(x/a) = (a * sin(pi * x) * sin(pi * x / a)) / (pi^2 * x^2)
// Assuming a Lanczos constant of 2.0, and scaling values to max out at x = +/- 1.5

void main()
{
	lowp vec4 frag_col = texture2D(texture0, v_center_texcoord) * 0.38026;

	frag_col += texture2D(texture0, v_one_step_left_texcoord) * 0.27667;
	frag_col += texture2D(texture0, v_one_step_right_texcoord) * 0.27667;

	frag_col += texture2D(texture0, v_two_step_left_texcoord) * 0.08074;
	frag_col += texture2D(texture0, v_two_step_right_texcoord) * 0.08074;

	frag_col += texture2D(texture0, v_three_step_left_texcoord) * -0.02612;
	frag_col += texture2D(texture0, v_three_step_right_texcoord) * -0.02612;

	frag_col += texture2D(texture0, v_four_step_left_texcoord) * -0.02143;
	frag_col += texture2D(texture0, v_four_step_right_texcoord) * -0.02143;
	
	gl_FragColor = frag_col;
}

);
