#include "sl_sprite.h"
#include "sl_matrix.h"
#include "sl_shader.h"

#include <render/render.h>

#include <stdint.h>

#define STRINGIFY(A)  #A
#include "sprite.vert"
#include "sprite.frag"

#define MAX_COMMBINE		1024

#define BUFFER_OFFSET(f) ((intptr_t)&(((struct vertex*)NULL)->f))

struct vertex {
	float vx, vy;
	float tx, ty;
	uint32_t color, additive;
};

struct shader_state {
	int shader;

	int projection_idx, modeview_idx;
	struct sl_matrix modelview_mat, projection_mat;

	uint32_t color, additive;
};

static struct shader_state S;

void 
sl_sprite_init() {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	uint16_t idxs[6 * MAX_COMMBINE];
	for (int i = 0; i < MAX_COMMBINE; ++i) {
		idxs[i*6] = i*4;
		idxs[i*6+1] = i*4+1;
		idxs[i*6+2] = i*4+2;
		idxs[i*6+3] = i*4;
		idxs[i*6+4] = i*4+2;
		idxs[i*6+5] = i*4+3;
	}
	sl_shader_create_index_buffer(s, 6 * MAX_COMMBINE, sizeof(uint16_t), idxs);	

	sl_shader_create_vertex_buffer(s, 4 * MAX_COMMBINE, sizeof(struct vertex));

	struct vertex_attrib va[4] = {
		{ "position", 0, 2, sizeof(float), BUFFER_OFFSET(vx) },
		{ "texcoord", 0, 2, sizeof(float), BUFFER_OFFSET(tx) },
		{ "color", 0, 4, sizeof(uint8_t), BUFFER_OFFSET(color) },
		{ "additive", 0, 4, sizeof(uint8_t), BUFFER_OFFSET(additive) },
	};
	sl_shader_create_vertex_layout(s, sizeof(va)/sizeof(va[0]), va);

	sl_shader_load(s, sprite_vert, sprite_frag);

	S.shader = s;

	S.projection_idx = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modeview_idx = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
	sl_matrix_identity(&S.projection_mat);
	sl_matrix_identity(&S.modelview_mat);

	S.color = 0xffffffff;
	S.additive = 0x00000000;
}

void 
sl_sprite_bind() {
	sl_shader_bind(S.shader);
}

void 
sl_sprite_projection(int width, int height) {
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	sl_matrix_ortho(&S.projection_mat, -hw, hw, -hh, hh, 1, -1);
	sl_shader_set_uniform(S.shader, S.projection_idx, UNIFORM_FLOAT44, S.projection_mat.e);
}

void 
sl_sprite_modelview(float x, float y, float sx, float sy) {
	sl_matrix_set_scale(&S.modelview_mat, sx, sy);
	sl_matrix_set_translate(&S.modelview_mat, x * sx, y * sy);
	sl_shader_set_uniform(S.shader, S.modeview_idx, UNIFORM_FLOAT44, S.modelview_mat.e);
}

// void 
// sl_sprite_type(int type) {
// 	sl_shader_set_draw_mode(S.shader, type);
// }

void 
sl_sprite_draw(float* positions, float* texcoords, int texid) {
	struct vertex vb[4];
	for (int i = 0; i < 4; ++i) {
		struct vertex* v = &vb[i];
		v->vx = positions[i * 2];
		v->vy = positions[i * 2 + 1];
		v->tx = texcoords[i * 2];
		v->ty = texcoords[i * 2 + 1];
		v->color = S.color;
		v->additive = S.additive;
	}
	sl_shader_draw(S.shader, vb, 4);
}
