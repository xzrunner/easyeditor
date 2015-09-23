#include "spritepack.h"
#include "matrix.h"
#include "shader.h"
#include "array.h"
#include "texture.h"

#include <lua.h>
#include <lauxlib.h>

#include <stdint.h>
#include <string.h>

#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define CC_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

#define TAG_ID 1
#define TAG_COLOR 2
#define TAG_ADDITIVE 4
#define TAG_MATRIX 8
#define TAG_TOUCH 16
#define TAG_MATRIXREF 32
#define TAG_COLMAP 64

struct import_alloc {
	lua_State *L;
	char * buffer;
	int cap;
};

#define ialloc(a, s) ialloc_dbg(a, s, __LINE__)

static void *
ialloc_dbg(struct import_alloc *alloc, int size, int line) {
	if (size <= 0) {
		return NULL;
	}
	if (alloc->cap < size) {
		luaL_error(alloc->L, "import invalid stream, alloc failed on line %d. cap = %d, size = %d", line, alloc->cap, size);
	}
	void * ret = alloc->buffer;
	alloc->buffer += size;
	alloc->cap -= size;
	return ret;
}

struct import_stream {
	struct import_alloc *alloc;
	struct sprite_pack *pack;
	const char * stream;
	size_t size;
	struct matrix *matrix;
	int matrix_n;
	int maxtexture;
	// for debug
	int current_id;
};

static int
import_byte(struct import_stream *is) {
	if (is->size == 0) {
		luaL_error(is->alloc->L, "Invalid import stream (%d)", is->current_id);
	}
	uint8_t c = (uint8_t)*(is->stream);
	++is->stream;
	--is->size;

	return c;
}

static int
import_word(struct import_stream *is) {
	if (is->size < 2) {
		luaL_error(is->alloc->L, "Invalid import stream (%d)", is->current_id);
	}
	uint8_t low = (uint8_t)*(is->stream);
	uint8_t high = (uint8_t)*(is->stream+1);
	is->stream += 2;
	is->size -= 2;

	return low | (uint32_t)high << 8;
}

static int
import_signed_word(struct import_stream *is) {
	if (is->size < 2) {
		luaL_error(is->alloc->L, "Invalid import stream (%d)", is->current_id);
	}
	uint8_t low = (uint8_t)*(is->stream);
	int8_t high = (int8_t)*(is->stream+1);
	is->stream += 2;
	is->size -= 2;

	return low | (int32_t)high << 8;
}

static int32_t
import_int32(struct import_stream *is) {
	if (is->size < 4) {
		luaL_error(is->alloc->L, "Invalid import stream (%d)",is->current_id);
	}
	uint8_t b[4];
	b[0] = (uint8_t)*(is->stream);
	b[1] = (uint8_t)*(is->stream+1);
	b[2] = (uint8_t)*(is->stream+2);
	b[3] = (uint8_t)*(is->stream+3);
	is->stream += 4;
	is->size -= 4;

	return (int32_t)(b[0] | (uint32_t)b[1]<<8 | (uint32_t)b[2]<<16 | (uint32_t)b[3]<<24);
}

static uint32_t
import_color(struct import_stream *is) {
	if (is->size < 4) {
		luaL_error(is->alloc->L, "Invalid import stream (%d) where import color", is->current_id);
	}
	uint8_t b[4];
	b[0] = (uint8_t)*(is->stream);
	b[1] = (uint8_t)*(is->stream+1);
	b[2] = (uint8_t)*(is->stream+2);
	b[3] = (uint8_t)*(is->stream+3);
	is->stream += 4;
	is->size -= 4;

	return (b[0] | (uint32_t)b[1]<<8 | (uint32_t)b[2]<<16 | (uint32_t)b[3]<<24);
}

static int
get_texid(struct import_stream *is, int texid) {
	if (texid < 0 || texid >= is->maxtexture) {
		return -1;
	}
	return is->pack->tex[texid];
}

static int
texture_coord(int id, float x, float y, uint16_t *u, uint16_t *v) {
	*u = (uint16_t)x;
	*v = (uint16_t)y;
	return 1;
}


static void
import_picture(struct import_stream *is) {
	int n = import_word(is);
	struct pack_picture * pp = (struct pack_picture *)ialloc(is->alloc, SIZEOF_PICTURE + n * SIZEOF_QUAD);
	pp->n = n;
	int i,j;
	for (i=0;i<n;i++) {
		struct pack_quad * q = &pp->rect[i];
		int texid = import_byte(is);
		q->texid = get_texid(is, texid);
		for (j=0;j<8;j+=2) {
			float x = (float)import_word(is);
			float y = (float)import_word(is);
			// todo: check the return value
			texture_coord(q->texid, x, y, &q->texture_coord[j], &q->texture_coord[j+1]);
		}
		for (j=0;j<8;j++) {
			q->screen_coord[j] = import_int32(is);
		}
	}
}

static void
import_polygon(struct import_stream *is) {
	int n = import_byte(is);
	struct pack_polygon * pp = (struct pack_polygon *)ialloc(is->alloc, SIZEOF_POLYGON + n * SIZEOF_POLY);
	pp->n = n;
	int i,j;
	for (i=0;i<n;i++) {
		struct pack_poly * p = &pp->poly[i];
		int texid = import_byte(is);
		p->texid = get_texid(is, texid);
		p->n = import_byte(is);
		p->texture_coord = (uint16_t *)ialloc(is->alloc, p->n * 2 * sizeof(*p->texture_coord));
		p->screen_coord = (int32_t *)ialloc(is->alloc, p->n * 2 * sizeof(uint32_t));
		for (j=0;j<p->n*2;j+=2) {
			float x = (float)import_word(is);
			float y = (float)import_word(is);
			// todo: check the return value
			texture_coord(p->texid, x, y, &p->texture_coord[j], &p->texture_coord[j+1]);
		}
		for (j=0;j<p->n*2;j++) {
			p->screen_coord[j] = import_int32(is);
		}
	}
}

static const char *
import_string(struct import_stream *is) {
	int n = import_byte(is);
	if (n==255) {
		return NULL;
	}
	if (is->size < n) {
		luaL_error(is->alloc->L, "Invalid stream (%d): read string failed", is->current_id);
	}
	char * buf = (char *)ialloc(is->alloc, (n+1+3) & ~3);
	memcpy(buf, is->stream, n);
	buf[n] = 0;
	is->stream += n;
	is->size -= n;

	return buf;
}

static void
import_frame(struct pack_frame * pf, struct import_stream *is, int maxc) {
	int n = import_word(is);
	int i;
	pf->part = (struct pack_part *)ialloc(is->alloc, n * SIZEOF_PART);
	pf->n = n;
	for (i=0;i<n;i++) {
		struct pack_part *pp = &pf->part[i];
		int tag = import_byte(is);
		if (tag & TAG_ID) {
			pp->component_id = import_word(is);
			if (pp->component_id >= maxc) {
				luaL_error(is->alloc->L, "Invalid stream (%d): frame part has wrong component id", is->current_id);
			}
		} else {
			luaL_error(is->alloc->L, "Invalid stream (%d): frame part need an id", is->current_id);
		}
		if (tag & TAG_MATRIX) {
			pp->t.mat = (struct matrix *)ialloc(is->alloc, SIZEOF_MATRIX);
			int32_t *m = pp->t.mat->m;
			int j;
			for (j=0;j<6;j++) {
				m[j] = import_int32(is);
			}
		} else if (tag & TAG_MATRIXREF) {
			int ref = import_int32(is);
			if (ref >= is->matrix_n) {
				luaL_error(is->alloc->L, "Invalid stream (%d): no martix ref %d", is->current_id, ref);
			}
			pp->t.mat = &is->matrix[ref];
		} else {
			pp->t.mat = NULL;
		}
		if (tag & TAG_COLOR) {
			pp->t.color = import_color(is);
		} else {
			pp->t.color = 0xffffffff;
		}
		if (tag & TAG_ADDITIVE) {
			pp->t.additive = import_color(is);
		} else {
			pp->t.additive = 0;
		}
		if (tag & TAG_TOUCH) {
			import_word(is);
			pp->touchable = 1;
		} else {
			pp->touchable = 0;
		}
		if (tag & TAG_COLMAP) {
			pp->t.rmap = import_color(is);
			pp->t.gmap = import_color(is);
			pp->t.bmap = import_color(is);
		} else {
			pp->t.rmap = 0xff0000ff;
			pp->t.gmap = 0x00ff00ff;
			pp->t.bmap = 0x0000ffff;
		}
		// todo: support other program
		pp->t.program = PROGRAM_DEFAULT;
	}
}

static void
import_animation(struct import_stream *is) {
	int component = import_word(is);
	struct pack_animation * pa = (struct pack_animation *)ialloc(is->alloc, SIZEOF_ANIMATION + component * SIZEOF_COMPONENT);
	pa->component_number = component;
	int i;
	for (i=0;i<component;i++) {
		int id = import_word(is);
		if (id != ANCHOR_ID && id >= is->pack->n) {
			luaL_error(is->alloc->L, "Invalid stream (%d): wrong id %d", is->current_id, id);
		}
		pa->component[i].id = id;
		pa->component[i].name = import_string(is);
	}
	pa->action_number = import_word(is);
	pa->action = (struct pack_action *)ialloc(is->alloc, SIZEOF_ACTION * pa->action_number);
	int frame = 0;
	for (i=0;i<pa->action_number;i++) {
		pa->action[i].name = import_string(is);
		pa->action[i].number = import_word(is);
		pa->action[i].start_frame = frame;
		frame += pa->action[i].number;
	}
	pa->frame_number = import_word(is);
	pa->frame = (struct pack_frame *)ialloc(is->alloc, SIZEOF_FRAME * pa->frame_number);
	for (i=0;i<pa->frame_number;i++) {
		import_frame(&pa->frame[i], is, component);
	}
}

static void
import_label(struct import_stream *is) {
	struct pack_label * pl = (struct pack_label *)ialloc(is->alloc, SIZEOF_LABEL);
	pl->align = import_byte(is);
	pl->color = import_color(is);
	pl->size = import_word(is);
	pl->width = import_word(is);
	pl->height = import_word(is);
	pl->edge = import_byte(is);
	pl->space_w = import_byte(is);
	pl->space_h = import_byte(is);
	pl->auto_scale = import_byte(is);
}

static void
import_pannel(struct import_stream *is) {
	struct pack_pannel *pp = (struct pack_pannel *)ialloc(is->alloc, SIZEOF_PANNEL);
	pp->width = import_int32(is);
	pp->height = import_int32(is);
	pp->scissor = import_byte(is);
}

static void
import_particle3d(struct import_stream *is) {
	int n = import_word(is);

	int sz = SIZEOF_PARTICLE3D + SIZEOF_P3D_SYMBOL * n;
	struct pack_particle3d* pp = (struct pack_particle3d*)ialloc(is->alloc, sz);
	memset(pp, 0, sz);

	struct p3d_ps_config* cfg = &pp->cfg;

	cfg->symbol_count = n;
	cfg->symbols = (struct p3d_symbol*)(cfg + 1);
	for (int i = 0; i < n; ++i)
	{
		struct p3d_symbol* dst = &cfg->symbols[i];

		uint16_t id = import_word(is);
		dst->ud = (void*)(id << 16 | i);
	
		dst->scale_start = import_word(is) * 0.01f;
		dst->scale_end = import_word(is) * 0.01f;

		dst->angle = CC_DEGREES_TO_RADIANS(import_signed_word(is));
		dst->angle_var = CC_DEGREES_TO_RADIANS(import_word(is));

		uint32_t mul = import_color(is);
		dst->col_mul.a = (mul >> 24 & 0xff) / 255.0f;
		dst->col_mul.r = (mul >> 16 & 0xff) / 255.0f;
		dst->col_mul.g = (mul >> 8 & 0xff) / 255.0f;
		dst->col_mul.b = (mul & 0xff) / 255.0f;

		uint32_t add = import_color(is);
		dst->col_add.a = (add >> 24 & 0xff) / 255.0f;
		dst->col_add.r = (add >> 16 & 0xff) / 255.0f;
		dst->col_add.g = (add >> 8 & 0xff) / 255.0f;
		dst->col_add.b = (add & 0xff) / 255.0f;

		dst->alpha_start = import_word(is);
		dst->alpha_end = import_word(is);
	}

	int cap = import_word(is);

	cfg->emission_time = import_word(is) * 0.001f;
	cfg->count = import_word(is);

	cfg->life = import_word(is) * 0.001f;
	cfg->life_var = import_word(is) * 0.001f;

	cfg->hori = CC_DEGREES_TO_RADIANS(import_word(is));
	cfg->hori_var = CC_DEGREES_TO_RADIANS(import_word(is));
	cfg->vert = CC_DEGREES_TO_RADIANS(import_word(is));
	cfg->vert_var = CC_DEGREES_TO_RADIANS(import_word(is));

	cfg->spd = import_word(is);
	cfg->spd_var = import_word(is);
	cfg->angular_spd = CC_DEGREES_TO_RADIANS(import_signed_word(is));
	cfg->angular_spd_var = CC_DEGREES_TO_RADIANS(import_word(is));

	cfg->dis_region = import_word(is);
	cfg->dis_region_var = import_word(is);
	cfg->dis_spd = import_word(is);
	cfg->dis_spd_var = import_word(is);

	cfg->gravity = import_signed_word(is);

	cfg->linear_acc = import_signed_word(is);
	cfg->linear_acc_var = import_word(is);

	cfg->fadeout_time = import_word(is) * 0.001f;

	cfg->bounce = import_byte(is);

	cfg->start_radius = import_word(is);
	cfg->is_start_radius_3d = import_byte(is);

	cfg->orient_to_movement = import_byte(is);

	// todo dir
	cfg->dir.x = 0;
	cfg->dir.y = 0;
	cfg->dir.z = 1;

	// create spr

	pp->spr.last_frame = -1;

	int particles_sz = SIZEOF_P3D_PARTICLE_SYSTEM + cap * SIZEOF_P3D_PARTICLE;
	void* mem = ialloc(is->alloc, particles_sz);
	struct p3d_particle_system* ps = p3d_create_with_mem(mem, cap, cfg);
	// todo
	ps->active = true;
	ps->loop = true;	
	pp->spr.ps = ps;
}

static void
import_particle2d(struct import_stream *is) {
	int n = import_word(is);

	int sz = SIZEOF_PARTICLE2D + SIZEOF_P2D_SYMBOL * n;
	struct pack_particle2d* pp = (struct pack_particle2d*)ialloc(is->alloc, sz);
	memset(pp, 0, sz);

	struct p2d_ps_config* cfg = &pp->cfg;

	cfg->symbol_count = n;
	cfg->symbols = (struct p2d_symbol*)(cfg + 1);
	for (int i = 0; i < n; ++i)
	{
		struct p2d_symbol* dst = &cfg->symbols[i];

		uint16_t id = import_word(is);
		dst->ud = (void*)(id << 16 | i);

		dst->angle_start = CC_DEGREES_TO_RADIANS(import_word(is));
		dst->angle_end = CC_DEGREES_TO_RADIANS(import_word(is));

		dst->scale_start = import_word(is) * 0.01f;
		dst->scale_end = import_word(is) * 0.01f;

		uint32_t col_mul_start = import_color(is);
		dst->col_mul_start.a = (col_mul_start >> 24 & 0xff) / 255.0f;
		dst->col_mul_start.r = (col_mul_start >> 16 & 0xff) / 255.0f;
		dst->col_mul_start.g = (col_mul_start >> 8 & 0xff) / 255.0f;
		dst->col_mul_start.b = (col_mul_start & 0xff) / 255.0f;

		uint32_t col_mul_end = import_color(is);
		dst->col_mul_end.a = (col_mul_end >> 24 & 0xff) / 255.0f;
		dst->col_mul_end.r = (col_mul_end >> 16 & 0xff) / 255.0f;
		dst->col_mul_end.g = (col_mul_end >> 8 & 0xff) / 255.0f;
		dst->col_mul_end.b = (col_mul_end & 0xff) / 255.0f;

		uint32_t col_add_start = import_color(is);
		dst->col_add_start.a = (col_add_start >> 24 & 0xff) / 255.0f;
		dst->col_add_start.r = (col_add_start >> 16 & 0xff) / 255.0f;
		dst->col_add_start.g = (col_add_start >> 8 & 0xff) / 255.0f;
		dst->col_add_start.b = (col_add_start & 0xff) / 255.0f;

		uint32_t col_add_end = import_color(is);
		dst->col_add_end.a = (col_add_end >> 24 & 0xff) / 255.0f;
		dst->col_add_end.r = (col_add_end >> 16 & 0xff) / 255.0f;
		dst->col_add_end.g = (col_add_end >> 8 & 0xff) / 255.0f;
		dst->col_add_end.b = (col_add_end & 0xff) / 255.0f;
	}

	int cap = import_word(is);

	cfg->emission_time = import_word(is) * 0.001f;
	cfg->count = import_word(is);

	cfg->life = import_word(is) * 0.001f;
	cfg->life_var = import_word(is) * 0.001f;

	cfg->position.x = import_word(is);
	cfg->position.y = import_word(is);
	cfg->position_var.x = import_word(is);
	cfg->position_var.y = import_word(is);

	cfg->direction = CC_DEGREES_TO_RADIANS(import_word(is));
	cfg->direction_var = CC_DEGREES_TO_RADIANS(import_word(is));

	cfg->mode_type = import_byte(is);

	if (cfg->mode_type == P2D_MODE_GRAVITY)
	{
		cfg->mode.A.gravity.x = import_word(is);
		cfg->mode.A.gravity.y = import_word(is);

		cfg->mode.A.speed = import_word(is);
		cfg->mode.A.speed_var = import_word(is);

		cfg->mode.A.tangential_accel = import_word(is);
		cfg->mode.A.tangential_accel_var = import_word(is);
		cfg->mode.A.radial_accel = import_word(is);
		cfg->mode.A.radial_accel_var = import_word(is);

		cfg->mode.A.rotation_is_dir = import_byte(is);
	}
	else if (cfg->mode_type == P2D_MODE_RADIUS)
	{
		cfg->mode.B.start_radius = import_byte(is);
		cfg->mode.B.start_radius_var = import_byte(is);
		cfg->mode.B.end_radius = import_byte(is);
		cfg->mode.B.end_radius_var = import_byte(is);

		cfg->mode.B.direction_delta = import_byte(is);
		cfg->mode.B.direction_delta_var = import_byte(is);
	}
	else if (cfg->mode_type == P2D_MODE_SPD_COS)
	{
		cfg->mode.C.speed = import_byte(is);
		cfg->mode.C.speed_var = import_byte(is);

		cfg->mode.C.cos_amplitude = import_byte(is);
		cfg->mode.C.cos_amplitude_var = import_byte(is);
		cfg->mode.C.cos_frequency = import_byte(is);
		cfg->mode.C.cos_frequency_var = import_byte(is);
	}

	// create spr

	pp->spr.last_frame = -1;

	int particles_sz = SIZEOF_P2D_PARTICLE_SYSTEM + cap * SIZEOF_P2D_PARTICLE;
	void* mem = ialloc(is->alloc, particles_sz);
	struct p2d_particle_system* ps = p2d_create_with_mem(mem, cap, cfg);
	// todo
	ps->is_active = true;
	ps->is_loop = true;
	pp->spr.ps = ps;
}

static void
import_matrix_chunk(struct import_stream *is) {
	if (is->matrix)
		luaL_error(is->alloc->L, "Invalid stream : only one matrix chunk support");
	int n = import_int32(is);
	is->matrix = (struct matrix *)ialloc(is->alloc, n * SIZEOF_MATRIX);
	is->matrix_n = n;
	int i,j;
	for (i=0;i<n;i++) {
		struct matrix *m = &is->matrix[i];
		for (j=0;j<6;j++) {
			m->m[j] = import_int32(is);
		}
	}
}

static void
import_sprite(struct import_stream *is) {
	int id = import_word(is);
	int type = import_byte(is);
	if (type == TYPE_MATRIX) {
		import_matrix_chunk(is);
		return;
	}
	if (id <0 || id >= is->pack->n) {
		luaL_error(is->alloc->L, "Invalid stream : wrong id %d", id);
	}
	is->current_id = id;
	is->pack->type[id] = type;
	if (is->pack->data[id]) {
		luaL_error(is->alloc->L, "Invalid stream : duplicate id %d", id);
	}
	is->pack->data[id] = is->alloc->buffer;
	switch (type) {
	case TYPE_PICTURE:
		import_picture(is);
		break;
	case TYPE_ANIMATION:
		import_animation(is);
		break;
	case TYPE_POLYGON:
		import_polygon(is);
		break;
	case TYPE_LABEL:
		import_label(is);
		break;
	case TYPE_PANNEL:
		import_pannel(is);
		break;
	case TYPE_PARTICLE3D:
		import_particle3d(is);
		break;
	case TYPE_PARTICLE2D:
		import_particle2d(is);
		break;
	default:
		luaL_error(is->alloc->L, "Invalid stream : Unknown type %d, id=%d", type, id);
		break;
	}
}

/*
	table/number texture
	integer maxid
	integer size
	string data
		lightuserdata data
		integer data_sz

	ret: userdata sprite_pack
 */

struct sprite_pack* 
ej_pkg_import(void* data, int sz, int tex, int max_id, int cap) {
	struct import_alloc alloc;
	alloc.L = NULL;
	alloc.buffer = malloc(cap);
	alloc.cap = cap;

	struct sprite_pack *pack = (struct sprite_pack *)ialloc(&alloc, SIZEOF_PACK + tex * sizeof(int));
	pack->n = max_id + 1;
	int align_n = (pack->n + 3) & ~3;
	pack->type = (uint8_t *)ialloc(&alloc, align_n * sizeof(uint8_t));
	memset(pack->type, 0, align_n * sizeof(uint8_t));
	pack->data = (void **)ialloc(&alloc, pack->n * SIZEOF_POINTER);
	memset(pack->data, 0, pack->n * SIZEOF_POINTER);
	for (int i = 0; i < tex; ++i) {
		pack->tex[i] = i;
	}

	struct import_stream is;
	is.alloc = &alloc;
	is.pack = pack;
	is.maxtexture = tex;
	is.current_id = -1;
	is.matrix = NULL;
	is.matrix_n = 0;
	is.stream = data;
	is.size = sz;

	while (is.size != 0) {
		import_sprite(&is);
	}

	// 	if (is.alloc->cap != 0) {
	// 		luaL_error(L, "Invalid import stream: unpack size");
	// 	}

	return pack;
}