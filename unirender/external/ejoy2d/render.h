#ifdef __cplusplus
extern "C"
{
#endif

#ifndef ejoy3d_render_h
#define ejoy3d_render_h

#include <stdint.h>

typedef unsigned int RID;

struct render;

struct render_init_args {
	int max_buffer;
	int max_layout;
	int max_target;
	int max_texture;
	int max_shader;
};

struct vertex_attrib {
	const char * name;
	int vbslot;
	int n;
	int size;
	int offset;
};

struct shader_init_args {
	const char * vs;
	const char * fs;
	int texture;
	const char **texture_uniform;
};

enum EJ_RENDER_OBJ {
	EJ_INVALID = 0,
	EJ_VERTEXLAYOUT = 1,
	EJ_VERTEXBUFFER = 2,
	EJ_INDEXBUFFER = 3,
	EJ_TEXTURE = 4,
	EJ_TARGET = 5,
	EJ_SHADER = 6,
};

enum EJ_TEXTURE_TYPE {
	EJ_TEXTURE_2D = 0,
	EJ_TEXTURE_CUBE,
};

enum EJ_TEXTURE_FORMAT {
	EJ_TEXTURE_INVALID = 0,
	EJ_TEXTURE_RGBA8,
	EJ_TEXTURE_RGBA4,
	EJ_TEXTURE_RGB,
	EJ_TEXTURE_RGB565,
	EJ_TEXTURE_A8,
	EJ_TEXTURE_DEPTH,	// use for render target
	EJ_TEXTURE_PVR2,
	EJ_TEXTURE_PVR4,
	EJ_TEXTURE_ETC1,
	EJ_TEXTURE_ETC2,
};

enum EJ_BLEND_FORMAT {
	EJ_BLEND_DISABLE = 0,
	EJ_BLEND_ZERO,
	EJ_BLEND_ONE,
	EJ_BLEND_SRC_COLOR,
	EJ_BLEND_ONE_MINUS_SRC_COLOR,
	EJ_BLEND_SRC_ALPHA,
	EJ_BLEND_ONE_MINUS_SRC_ALPHA,
	EJ_BLEND_DST_ALPHA,
	EJ_BLEND_ONE_MINUS_DST_ALPHA,
	EJ_BLEND_DST_COLOR,
	EJ_BLEND_ONE_MINUS_DST_COLOR,
	EJ_BLEND_SRC_ALPHA_SATURATE,
};

enum EJ_BLEND_FUNC {
	EJ_BLEND_FUNC_ADD = 0,
	EJ_BLEND_FUNC_SUBTRACT,
	EJ_BLEND_FUNC_REVERSE_SUBTRACT,
};

enum EJ_DEPTH_FORMAT {
	EJ_DEPTH_DISABLE = 0,
	EJ_DEPTH_LESS_EQUAL,
	EJ_DEPTH_LESS,
	EJ_DEPTH_EQUAL,
	EJ_DEPTH_GREATER,
	EJ_DEPTH_GREATER_EQUAL,
	EJ_DEPTH_ALWAYS,
};

enum EJ_CLEAR_MASK {
	EJ_MASKC = 0x1,
	EJ_MASKD = 0x2,
	EJ_MASKS = 0x4,
};

enum EJ_UNIFORM_FORMAT {
	EJ_UNIFORM_INVALID = 0,
	EJ_UNIFORM_FLOAT1,
	EJ_UNIFORM_FLOAT2,
	EJ_UNIFORM_FLOAT3,
	EJ_UNIFORM_FLOAT4,
	EJ_UNIFORM_FLOAT33,
	EJ_UNIFORM_FLOAT44,
	EJ_UNIFORM_INT1,
};

enum EJ_DRAW_MODE {
	EJ_DRAW_POINTS = 0,
	EJ_DRAW_LINES,
	EJ_DRAW_LINE_LOOP,
	EJ_DRAW_LINE_STRIP,
	EJ_DRAW_TRIANGLES,
	EJ_DRAW_TRIANGLE_STRIP,
	EJ_DRAW_TRIANGLE_FAN,
};

enum EJ_CULL_MODE {
	EJ_CULL_DISABLE = 0,
	EJ_CULL_FRONT,
	EJ_CULL_BACK,
};

int render_version(struct render *R);
int render_size(struct render_init_args *args);
struct render * render_init(struct render_init_args *args, void * buffer, int sz);
void render_exit(struct render * R);

void render_set(struct render *R, enum EJ_RENDER_OBJ what, RID id, int slot);
RID render_get(struct render *R, enum EJ_RENDER_OBJ what, int slot);
void render_release(struct render *R, enum EJ_RENDER_OBJ what, RID id);

RID render_register_vertexlayout(struct render *R, int n, struct vertex_attrib * attrib);

// what should be EJ_VERTEXBUFFER or EJ_INDEXBUFFER
RID render_buffer_create(struct render *R, enum EJ_RENDER_OBJ what, const void *data, int n, int stride);
void render_buffer_update(struct render *R, RID id, const void * data, int n);

RID render_texture_create(struct render *R, int width, int height, enum EJ_TEXTURE_FORMAT format, enum EJ_TEXTURE_TYPE type, int mipmap);
void render_texture_update(struct render *R, RID id, int width, int height, const void *pixels, int slice, int miplevel);
// subupdate only support slice 0, miplevel 0
void render_texture_subupdate(struct render *R, RID id, const void *pixels, int x, int y, int w, int h);

RID render_target_create(struct render *R, int width, int height, enum EJ_TEXTURE_FORMAT format);
// render_release EJ_TARGET would not release the texture attachment
RID render_target_texture(struct render *R, RID rt);

RID render_shader_create(struct render *R, struct shader_init_args *args);
void render_shader_bind(struct render *R, RID id);
int render_shader_locuniform(struct render *R, const char * name);
void render_shader_setuniform(struct render *R, int loc, enum EJ_UNIFORM_FORMAT format, const float *v);

void render_setviewport(int x, int y, int width, int height );
void render_setscissor(int x, int y, int width, int height );

void render_set_blendfunc(struct render *R, enum EJ_BLEND_FORMAT src, enum EJ_BLEND_FORMAT dst);
void render_set_blendeq(struct render *R, enum EJ_BLEND_FUNC eq);
void render_setdepth(struct render *R, enum EJ_DEPTH_FORMAT d);
void render_setcull(struct render *R, enum EJ_CULL_MODE c);
void render_enabledepthmask(struct render *R, int enable);
void render_enablescissor(struct render *R, int enable);

void render_state_reset(struct render *R);

void render_clear(struct render *R, enum EJ_CLEAR_MASK mask, unsigned long argb);
void render_draw_elements(struct render *R, enum EJ_DRAW_MODE mode, int fromidx, int ni);
void render_draw_arrays(struct render *R, enum EJ_DRAW_MODE mode, int fromidx, int ni);

// todo
int render_get_texture_gl_id(struct render *R, RID id);

int render_query_target();

void render_clear_texture_cache(struct render* R);

#endif

#ifdef __cplusplus
}
#endif