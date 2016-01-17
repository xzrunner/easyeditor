#include "sl_shader.h"
#include "sl_vertexbuffer.h"

#include <render/render.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_SHADER 16

#define MAX_UNIFORM 16
#define MAX_TEXTURE_CHANNEL 8

struct uniform {
	int loc;
	int offset;
	enum UNIFORM_FORMAT type;
};

struct shader {
	RID prog;

//	struct material * material;
	int texture_number;

	// uniform
	int uniform_number;
	struct uniform uniform[MAX_UNIFORM];
	bool reset_uniform;
	bool uniform_change[MAX_UNIFORM];
	float uniform_value[MAX_UNIFORM * 16];

	RID vertex_buffer;
	RID index_buffer;
	RID layout;

	enum DRAW_MODE draw_mode;
	struct sl_vertexbuffer* vb;
};

struct shader_mgr {
	struct render* R;

	int curr_shader;
	int shader_size;
	struct shader shader[MAX_SHADER];

 	int tex[MAX_TEXTURE_CHANNEL];
// 	int blendchange;
// 	int drawcall;

// 	RID layout;
// 	RID target;
// 	struct render_buffer vb;
};

static struct shader_mgr* S = NULL;

void 
sl_shader_init() {
	if (S) return;

	struct shader_mgr* sm = (struct shader_mgr*)malloc(sizeof(*sm));
	memset(sm, 0 , sizeof(*sm));

	struct render_init_args RA;
	// todo: config these args
	RA.max_buffer = 128;
	RA.max_layout = 4;
	RA.max_target = 128;
	RA.max_texture = 256;
	RA.max_shader = MAX_SHADER;

	int smz = render_size(&RA);
	sm->R = (struct render*)malloc(smz);
	sm->R = render_init(&RA, sm->R, smz);

	sm->curr_shader = -1;
	sm->shader_size = 0;
//	sm->blendchange = 0;
	render_setblend(sm->R, BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA);

	S = sm;
}

int 
sl_shader_create() {
	if (S->shader_size >= MAX_SHADER) {
		return -1;
	} else {
		int id = S->shader_size++;
		struct shader* s = &S->shader[id];
		struct render* R = S->R;
		if (s->prog) {
			render_release(R, SHADER, s->prog);
			s->prog = 0;
		}
		memset(s, 0, sizeof(*s));
		return id;
	}
}

void 
sl_shader_load(int id, const char* vs, const char* fs) {
	struct shader* s = &S->shader[id];
	struct render* R = S->R;
	struct shader_init_args args;
	args.vs = vs;
	args.fs = fs;
// 	args.texture = texture;
// 	args.texture_uniform = texture_uniform_name;
	s->prog = render_shader_create(R, &args);
	render_shader_bind(R, s->prog);
	render_shader_bind(R, 0);
//	p->texture_number = texture;
	S->curr_shader = -1;
}

static inline void
_commit(struct shader* s) {
	struct sl_vertexbuffer* vb = s->vb;
	if (vb->n == 0) {
		return;
	}
	render_buffer_update(S->R, s->vertex_buffer, vb->buf, vb->n);
	render_draw_arrays(S->R, s->draw_mode, 0, vb->n);
	vb->n = 0;
}

void 
sl_shader_set_draw_mode(int id, enum DRAW_MODE dm) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];
	if (s->draw_mode != dm) {
		_commit(s);
		s->draw_mode = dm;
	}
}

void 
sl_shader_set_texture(int id, int channel) {
	assert(channel < MAX_TEXTURE_CHANNEL);
	if (S->tex[channel] != id) {
		sl_shader_flush();
		S->tex[channel] = id;
		render_set(S->R, TEXTURE, id, channel);
	}
}

void 
sl_shader_create_vertex_buffer(int id, int n, int stride) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];

	struct render* R = S->R;
	RID vertex_buffer = render_buffer_create(R, VERTEXBUFFER, NULL, n, stride);
	render_set(R, VERTEXBUFFER, vertex_buffer, 0);

	s->vertex_buffer = vertex_buffer;
	s->vb = sl_vb_create(stride, n);
}

void 
sl_shader_create_index_buffer(int id, int n, int stride, const void* data) {
	assert(id >= 0 && id < MAX_SHADER);

	struct render* R = S->R;
	RID index_buffer = render_buffer_create(R, INDEXBUFFER, data, n, stride);
	render_set(R, INDEXBUFFER, index_buffer, 0);

	S->shader[id].index_buffer = index_buffer;
}

void 
sl_shader_create_vertex_layout(int id, int n, struct vertex_attrib* va) {
	assert(id >= 0 && id < MAX_SHADER);

	struct render* R = S->R;
	RID layout = render_register_vertexlayout(R, n, va);
	render_set(R, VERTEXLAYOUT, layout, 0);

	S->shader[id].layout = layout;
}

static void
_apply_uniform(struct shader* s) {
	struct render* R = S->R;
	for (int i = 0; i < s->uniform_number; ++i) {
		if (!s->uniform_change[i]) {
			continue;
		}
		struct uniform* u = &s->uniform[i];
		if (u->loc >= 0) {
			render_shader_setuniform(R, u->loc, u->type, s->uniform_value + u->offset);
		}
	}
	s->reset_uniform = true;
}

void 
sl_shader_bind(int id) {
	struct shader* s = &S->shader[id];
	if (S->curr_shader != id || s->reset_uniform) {
		_commit(s);
	}
	if (S->curr_shader != id) {
		S->curr_shader = id;
		render_shader_bind(S->R, s->prog);
		_apply_uniform(s);
	} else if (s->reset_uniform) {
		_apply_uniform(s);
	}
}

int 
sl_shader_add_uniform(int id, const char* name, enum UNIFORM_FORMAT t) {
	// reset current_program
	assert(id >=0 && id < MAX_SHADER);
	sl_shader_bind(id);
	struct shader* s = &S->shader[id];
	assert(s->uniform_number < MAX_UNIFORM);
	int loc = render_shader_locuniform(S->R, name);
	int index = s->uniform_number++;
	struct uniform* u = &s->uniform[index];
	u->loc = loc;
	u->type = t;
	if (index == 0) {
		u->offset = 0;
	} else {
		struct uniform* lu = &s->uniform[index-1];
		u->offset = lu->offset + sl_shader_uniform_size(lu->type);
	}
	if (loc < 0)
		return -1;
	return index;
}

void 
sl_shader_set_uniform(int id, int index, enum UNIFORM_FORMAT t, float* v) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];
	_commit(s);
	assert(index >= 0 && index < s->uniform_number);
	struct uniform* u = &s->uniform[index];
	assert(t == u->type);
	int n = sl_shader_uniform_size(t);
	memcpy(s->uniform_value + u->offset, v, n * sizeof(float));
	s->reset_uniform = true;
	s->uniform_change[index] = true;
}

int 
sl_shader_uniform_size(enum UNIFORM_FORMAT t) {
	int n = 0;
	switch(t) {
	case UNIFORM_INVALID:
		n = 0;
		break;
	case UNIFORM_FLOAT1:
		n = 1;
		break;
	case UNIFORM_FLOAT2:
		n = 2;
		break;
	case UNIFORM_FLOAT3:
		n = 3;
		break;
	case UNIFORM_FLOAT4:
		n = 4;
		break;
	case UNIFORM_FLOAT33:
		n = 9;
		break;
	case UNIFORM_FLOAT44:
		n = 16;
		break;
	}
	return n;
}

void 
sl_shader_draw(int id, void* data, int n) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];
	if (sl_vb_add(s->vb, data, n)) {
		_commit(s);
	}
}

void 
sl_shader_flush() {
	assert(S->curr_shader >= 0 && S->curr_shader < MAX_SHADER);
	struct shader* s = &S->shader[S->curr_shader];
	_commit(s);
}