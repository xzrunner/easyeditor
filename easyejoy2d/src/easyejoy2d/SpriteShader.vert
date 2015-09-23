static const char* SpriteVertShader = STRINGIFY(

attribute vec4 position;
attribute vec2 texcoord;
attribute vec4 color;
attribute vec4 additive;

varying vec2 v_texcoord;
varying vec4 v_color;
varying vec4 v_additive;

void main() {
    gl_Position = position + vec4(-1.0,1.0,0,0);
    v_texcoord = texcoord;
    v_color = color;
    v_additive = additive;
}

);