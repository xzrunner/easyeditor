static const char* sprite_map_frag = STRINGIFY(

varying vec2 v_texcoord;
varying vec4 v_rmap;
varying vec4 v_gmap;
varying vec4 v_bmap;

uniform sampler2D texture0;

bool neql(float a, float b) {
	return abs(a - b) > 0.0001;
}

void main() {
    vec4 tmp = texture2D(texture0, v_texcoord);

    float s = 1.2;
    vec4 r = vec4(tmp.r, 0, 0, 1);
    vec4 g = vec4(0, tmp.g, 0, 1);
    vec4 b = vec4(0, 0, tmp.b, 1);
    if ((neql(v_rmap.r, 1.0) || neql(v_rmap.g, 0.0) || neql(v_rmap.b, 0.0)) 
        && tmp.r > tmp.g * s && tmp.r > tmp.b * s) {
       r = tmp.r * v_rmap;
       g = tmp.g * v_rmap;
       b = tmp.b * v_rmap;
    }
    if ((neql(v_gmap.g, 1.0) || neql(v_gmap.r, 0.0) || neql(v_gmap.b, 0.0)) 
        && tmp.g > tmp.r * s && tmp.g > tmp.b * s) {
       g = tmp.g * v_gmap;
       r = tmp.r * v_gmap;
       b = tmp.b * v_gmap;
    }
    if ((neql(v_bmap.b, 1.0) || neql(v_bmap.r, 0.0) || neql(v_bmap.g, 0.0))
        && tmp.b > tmp.r * s && tmp.b > tmp.g * s) {
      b = tmp.b * v_bmap;
      r = tmp.r * v_bmap;
      g = tmp.g * v_bmap;
    }
    tmp.xyz = (r + g + b).xyz;

    gl_FragColor = tmp;
}

);
