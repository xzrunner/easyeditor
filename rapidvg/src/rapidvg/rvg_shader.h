#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rapid_vector_graphics_shader_h
#define rapid_vector_graphics_shader_h

#define TYPE_POINTS			0x0000
#define TYPE_LINES			0x0001
#define TYPE_LINE_LOOP		0x0002
#define TYPE_LINE_STRIP		0x0003
#define TYPE_TRIANGLES		0x0004
#define TYPE_TRIANGLE_STRIP	0x0005
#define TYPE_TRIANGLE_FAN	0x0006
#define TYPE_QUADS			0x0007
#define TYPE_QUAD_STRIP		0x0008
#define TYPE_POLYGON		0x0009

void rvg_shader_init();

void rvg_shader_load();
void rvg_shader_unload();

void rvg_shader_bind();
void rvg_shader_unbind();

void rvg_shader_projection(int width, int height);
void rvg_shader_modelview(float dx, float dy, float scale);

void rvg_shader_type(int type);

void rvg_shader_color(int color);

void rvg_shader_add_vertices(float* coords, int count);

void rvg_shader_flush();

#endif // rapid_vector_graphics_shader_h

#ifdef __cplusplus
}
#endif