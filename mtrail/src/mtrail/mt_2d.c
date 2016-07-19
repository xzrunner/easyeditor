#include "mt_2d.h"
#include "mt_array.h"

#include <logger.h>
#include <sm_c_calc.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_PARTICLE_SZ 10000
#define MAX_EMITTER_SZ	1000

static struct t2d_particle* PARTICLE_ARRAY = NULL;
static struct t2d_emitter*	EMITTER_ARRAY = NULL;

void (*RENDER_SYMBOL_FUNC)(void* symbol, float x, float y, float angle, uint8_t* mul_col, uint8_t* add_col, const void* ud);
void (*RENDER_SHAPE_FUNC)(const float* positions, const uint32_t* colors, int count);

void 
t2d_init() {
	int sz = sizeof(struct t2d_particle) * MAX_PARTICLE_SZ;
	PARTICLE_ARRAY = (struct t2d_particle*)malloc(sz);
	if (!PARTICLE_ARRAY) {
		LOGW("malloc err: t2d_init particle");
		return;
	}
	memset(PARTICLE_ARRAY, 0, sz);
	MT_ARRAY_INIT(PARTICLE_ARRAY, MAX_PARTICLE_SZ);

	sz = sizeof(struct t2d_emitter) * MAX_EMITTER_SZ;
	EMITTER_ARRAY = (struct t2d_emitter*)malloc(sz);
	if (!EMITTER_ARRAY) {
		LOGW("malloc err: t2d_init emitter");
		return;
	}
	memset(EMITTER_ARRAY, 0, sz);
	MT_ARRAY_INIT(EMITTER_ARRAY, MAX_EMITTER_SZ);
}

void 
t2d_regist_cb(void (*render_symbol_func)(void* symbol, float x, float y, float angle, uint8_t* mul_col, uint8_t* add_col, const void* ud),
			  void (*render_shape_func)(const float* positions, const uint32_t* colors, int count)) {
	RENDER_SYMBOL_FUNC = render_symbol_func;
	RENDER_SHAPE_FUNC = render_shape_func;
}

struct t2d_emitter* 
t2d_emitter_create(const struct t2d_emitter_cfg* cfg) {
	struct t2d_emitter* et;
	MT_ARRAY_ALLOC(EMITTER_ARRAY, et);
	if (!et) {
		return NULL;
	}
	memset(et, 0, sizeof(struct t2d_emitter));
	et->cfg = cfg;
	return et;
}

void 
t2d_emitter_release(struct t2d_emitter* et) {
	t2d_emitter_clear(et);
	free(et);
}

void 
t2d_emitter_clear(struct t2d_emitter* et) {
	struct t2d_particle* p = et->head;
	while (p) {
		struct t2d_particle* next = p->next;
		MT_ARRAY_FREE(PARTICLE_ARRAY, p);
		p = next;
	}

	et->head = NULL;
}

void 
t2d_emitter_start(struct t2d_emitter* et) {
	t2d_emitter_clear(et);

	et->active = true;
	et->particle_count = 0;
}

void 
t2d_emitter_stop(struct t2d_emitter* et) {
	et->active = false;
}

static inline void
_init_particle(struct t2d_emitter* et, struct t2d_particle* p) {
	uint32_t RANDSEED = rand();

	p->sym = (struct t2d_symbol*)(et->cfg->symbols + RANDSEED % et->cfg->symbol_count);

	p->lifetime = et->cfg->life_begin + et->cfg->life_offset * et->particle_count;
	p->life = p->lifetime;

	if (et->head) {
		struct sm_vec2 s = et->head->pos, e = p->pos;
		p->angle = sm_get_line_angle(&s, &e);
	} else {
		p->angle = 0;
	}
}

static inline bool
_add_particle_random(struct t2d_emitter* et, struct sm_vec2* pos) {
	if (!et->cfg->symbol_count) {
		return false;
	}

	if (et->head) {
		if (pos->x == et->head->pos.x ||
			pos->y == et->head->pos.y) {
			return false;
		}
	}

	struct t2d_particle* p;
	MT_ARRAY_ALLOC(PARTICLE_ARRAY, p);
	if (!p) {
		return false;
	}

	p->pos = *pos;
	_init_particle(et, p);

	p->next = NULL;
	if (!et->head) {
		et->head = p;
	} else {
		p->next = et->head;
		et->head = p;
	}

	return true;
}

static inline void
_remove_particle(struct t2d_emitter* et, struct t2d_particle* p) {
	MT_ARRAY_FREE(PARTICLE_ARRAY, p);
	--et->particle_count;
	if (p == et->head) {
		et->head = NULL;
	}
}

void 
t2d_emitter_update(struct t2d_emitter* et, float dt, struct sm_vec2* pos) {
	struct t2d_particle* prev = NULL;
	struct t2d_particle* curr = et->head;
	while (curr) {
		curr->life -= dt;
		if (curr->life > 0) {
			prev = curr;
			curr = curr->next;
		} else {
			if (prev) {
				prev->next = NULL;
			}
			
			struct t2d_particle* next = curr->next;
			do {
				_remove_particle(et, curr);
				curr = next;
				if (curr) {
					next = curr->next;
				} else {
					break;
				}
			} while (true);
			
			break;
		}
	}

	if (et->active) {
		if (et->particle_count < et->cfg->count) {
			if (_add_particle_random(et, pos)) {
				++et->particle_count;
			}
		}
	}
}

static void
_offset_segment(struct sm_vec2* s, struct sm_vec2* e, float half_width,
				struct sm_vec2* ls, struct sm_vec2* le,
				struct sm_vec2* rs, struct sm_vec2* re) {
	struct sm_vec2 off;
	sm_vec2_vector(&off, s, e);
	sm_vec2_normalize(&off);
	off.x *= half_width;
	off.y *= half_width;	

	struct sm_vec2 tmp;
	sm_rotate_vector_right_angle(&off, true, &tmp);	
	sm_vec2_add(ls, s, &tmp);
	sm_vec2_add(le, e, &tmp);
	sm_rotate_vector_right_angle(&off, false, &tmp);	
	sm_vec2_add(rs, s, &tmp);
	sm_vec2_add(re, e, &tmp);
}

static void
_add_shape_node(struct t2d_emitter* et, float* positions, uint32_t* colors, int* ptr, 
                struct sm_vec2* pos, struct t2d_particle* p) {
	positions[*ptr * 2] = pos->x;
	positions[*ptr * 2 + 1] = pos->y;

	struct mt_color col;
	float proc = (p->lifetime - p->life) / p->lifetime;
	col = p->sym->col_mul;
	if (p->life < et->cfg->fadeout_time) {
		col.a *= p->life / et->cfg->fadeout_time;
	}
	float alpha = proc * (p->sym->alpha_end - p->sym->alpha_start) + p->sym->alpha_start;
	col.a *= alpha;

	col.a = 0;
	colors[*ptr] = (col.a << 24) | (col.b << 16) | (col.g << 8) | col.r;
	++*ptr;
}

static void
_draw_shape(struct t2d_emitter* et, const void* ud) {
	if (et->particle_count < 2) {
		return;
	}

	int count = et->particle_count * 2;

	float positions[count * 2];
	uint32_t colors[count];

	struct sm_vec2 l0, l1, r0, r1;
	struct sm_vec2 l2, l3, r2, r3;

	const float MAX_HW = 5;

	int ptr = 0;
	struct t2d_particle *prev = et->head, *curr = prev->next;
	_offset_segment(&prev->pos, &curr->pos, MAX_HW, &l0, &l1, &r0, &r1);
	_add_shape_node(et, positions, colors, &ptr, &l0, prev);
	_add_shape_node(et, positions, colors, &ptr, &r0, prev);
	prev = curr;
	curr = curr->next;

	while (curr) {
		_offset_segment(&prev->pos, &curr->pos, MAX_HW, &l2, &l3, &r2, &r3);

		struct sm_vec2 lc, rc;
		sm_intersect_line_line(&l0, &l1, &l2, &l3, &lc);
		sm_intersect_line_line(&r0, &r1, &r2, &r3, &rc);
		_add_shape_node(et, positions, colors, &ptr, &lc, prev);
		_add_shape_node(et, positions, colors, &ptr, &rc, prev);

		l0 = l2;
		l1 = l3;
		r0 = r2;
		r1 = r3;

		prev = curr;
		curr = curr->next;
	}

	if (et->particle_count == 2) {
		_add_shape_node(et, positions, colors, &ptr, &l1, prev);
		_add_shape_node(et, positions, colors, &ptr, &r1, prev);
	} else {
		_add_shape_node(et, positions, colors, &ptr, &l3, prev);
		_add_shape_node(et, positions, colors, &ptr, &r3, prev);
	}

	RENDER_SHAPE_FUNC(positions, colors, count);
}

static void
_draw_symbol(struct t2d_emitter* et, const void* ud) {
	struct mt_color mul_col;

	struct t2d_particle* p = et->head;
	while (p) {
		float proc = (p->lifetime - p->life) / p->lifetime;

		mul_col = p->sym->col_mul;
		if (p->life < et->cfg->fadeout_time) {
			mul_col.a *= p->life / et->cfg->fadeout_time;
		}
		float alpha = proc * (p->sym->alpha_end - p->sym->alpha_start) + p->sym->alpha_start;
		mul_col.a *= alpha;

		RENDER_SYMBOL_FUNC(p->sym->mode.B.ud, p->pos.x, p->pos.y, p->angle, mul_col.rgba, p->sym->col_add.rgba, ud);
		p = p->next;
	}
}

void 
t2d_emitter_draw(struct t2d_emitter* et, const void* ud) {
	if (et->cfg->mode_type == T2D_MODE_SHAPE) {
		_draw_shape(et, ud);
	} else if (et->cfg->mode_type == T2D_MODE_SYMBOL) {
		_draw_symbol(et, ud);
	}
}