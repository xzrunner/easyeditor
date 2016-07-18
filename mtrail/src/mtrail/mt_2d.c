#include "mt_2d.h"
#include "mt_array.h"

#include <logger.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_PARTICLE_SZ 10000
#define MAX_EMITTER_SZ	1000

static struct t2d_particle* PARTICLE_ARRAY = NULL;
static struct t2d_emitter*	EMITTER_ARRAY = NULL;

void (*RENDER_FUNC)(void* symbol, float* mat, float x, float y, const void* ud);

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
t2d_regist_cb(void (*render_func)(void* symbol, float* mat, float x, float y, const void* ud)) {
	RENDER_FUNC = render_func;
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
	et->active = true;
	et->particle_count = 0;
}

static inline void
_init_particle(struct t2d_emitter* et, struct t2d_particle* p) {
	uint32_t RANDSEED = rand();

	p->sym = (struct t2d_symbol*)(et->cfg->symbols + RANDSEED % et->cfg->symbol_count);
}

static inline void
_add_particle_random(struct t2d_emitter* et, float* mat) {
	if (!et->cfg->symbol_count) {
		return;
	}

	struct t2d_particle* p;
	MT_ARRAY_ALLOC(PARTICLE_ARRAY, p);
	if (!p) {
		return;
	}

	memcpy(p->mat, mat, sizeof(p->mat));
	_init_particle(et, p);

	p->next = NULL;
	if (!et->head) {
		et->head = p;
	} else {
		p->next = et->head;
		et->head = p;
	}
}

void 
t2d_emitter_update(struct t2d_emitter* et, float dt, float* mat) {
	if (et->active) {
		if (et->particle_count < et->cfg->count) {
			_add_particle_random(et, mat);
			++et->particle_count;
		}
	}
}

static void
_draw_shape(struct t2d_emitter* et, const void* ud) {
	
}

static void
_draw_symbol(struct t2d_emitter* et, const void* ud) {
	struct t2d_particle* p = et->head;
	while (p) {
		RENDER_FUNC(p->sym->mode.B.ud, p->mat, p->pos.x, p->pos.y, ud);
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