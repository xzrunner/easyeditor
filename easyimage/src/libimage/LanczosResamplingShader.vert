static const char* LanczosVertShader = STRINGIFY(

attribute vec4 position;
attribute vec2 texcoord;

uniform mat4 u_projection;
uniform mat4 u_modelview;

uniform float u_texel_width;
uniform float u_texel_height;

varying vec2 v_center_texcoord;
varying vec2 v_one_step_left_texcoord;
varying vec2 v_two_step_left_texcoord;
varying vec2 v_three_step_left_texcoord;
varying vec2 v_four_step_left_texcoord;
varying vec2 v_one_step_right_texcoord;
varying vec2 v_two_step_right_texcoord;
varying vec2 v_three_step_right_texcoord;
varying vec2 v_four_step_right_texcoord;

void main()
{
	gl_Position = u_projection * u_modelview * position;
	
	float u_texel_width = 1.0 / 4096;
	float u_texel_height = 1.0 / 4096;

	vec2 firstOffset = vec2(u_texel_width, u_texel_height);
	vec2 secondOffset = vec2(2.0 * u_texel_width, 2.0 * u_texel_height);
	vec2 thirdOffset = vec2(3.0 * u_texel_width, 3.0 * u_texel_height);
	vec2 fourthOffset = vec2(4.0 * u_texel_width, 4.0 * u_texel_height);

	v_center_texcoord = texcoord;
	v_one_step_left_texcoord = texcoord - firstOffset;
	v_two_step_left_texcoord = texcoord - secondOffset;
	v_three_step_left_texcoord = texcoord - thirdOffset;
	v_four_step_left_texcoord = texcoord - fourthOffset;
	v_one_step_right_texcoord = texcoord + firstOffset;
	v_two_step_right_texcoord = texcoord + secondOffset;
	v_three_step_right_texcoord = texcoord + thirdOffset;
	v_four_step_right_texcoord = texcoord + fourthOffset;
}

);
