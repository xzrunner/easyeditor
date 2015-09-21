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


static int
limport(lua_State *L) {
	int max_id = (int)luaL_checkinteger(L, 2);
	int size = (int)luaL_checkinteger(L, 3);
	int tex;
	int tt = lua_type(L,1);
	if (tt == LUA_TNIL) {
		tex = 0;
	} else if (tt == LUA_TNUMBER) {
		tex = 1;
	} else {
		if (tt != LUA_TTABLE) {
			return luaL_error(L, "Need textures");
		}

		tex = lua_rawlen(L,1);
	}

	struct import_alloc alloc;
	alloc.L = L;
	alloc.buffer = (char *)lua_newuserdata(L, size);
	alloc.cap = size;

	struct sprite_pack *pack = (struct sprite_pack *)ialloc(&alloc, SIZEOF_PACK + tex * sizeof(int));
	pack->n = max_id + 1;
	int align_n = (pack->n + 3) & ~3;
	pack->type = (uint8_t *)ialloc(&alloc, align_n * sizeof(uint8_t));
	memset(pack->type, 0, align_n * sizeof(uint8_t));
	pack->data = (void **)ialloc(&alloc, pack->n * SIZEOF_POINTER);
	memset(pack->data, 0, pack->n * SIZEOF_POINTER);

	if (lua_istable(L,1)) {
		int i;
		for (i=0; i<tex; i++) {
			lua_rawgeti(L,1,i+1);
			pack->tex[i] = (int)luaL_checkinteger(L, -1);
			lua_pop(L,1);
		}
	} else if (!lua_isnil(L,1)) {
		pack->tex[0] = (int)lua_tointeger(L,1);
	}

	struct import_stream is;
	is.alloc = &alloc;
	is.pack = pack;
	is.maxtexture = tex;
	is.current_id = -1;
	is.matrix = NULL;
	is.matrix_n = 0;
	if (lua_isstring(L,4)) {
		is.stream = lua_tolstring(L, 4, &is.size);
	} else {
		is.stream = (const char *)lua_touserdata(L, 4);
		if (is.stream == NULL) {
			return luaL_error(L, "Need const char *");
		}
		is.size = luaL_checkinteger(L, 5);
	}

	while (is.size != 0) {
		import_sprite(&is);
	}

// 	if (is.alloc->cap != 0) {
// 		luaL_error(L, "Invalid import stream: unpack size");
// 	}

	return 1;
}

static int32_t
readinteger(lua_State *L, int idx) {
	if (lua_isinteger(L, idx)) {
		return (int32_t)lua_tointeger(L, idx);
	} else {
		lua_Number n = luaL_checknumber(L, idx);
		return (int32_t)n;
	}
}

static int
lpackbyte(lua_State *L) {
	int n = (int)readinteger(L, 1);
	if (n < 0 || n > 255) {
		return luaL_error(L, "pack byte %d", n);
	}
	uint8_t buf[1] = { (uint8_t)n };
	lua_pushlstring(L, (char *)buf, 1);
	return 1;
}

static int
lpackword(lua_State *L) {
	int n = (int)readinteger(L, 1);
	if (n < 0 || n > 0xffff) {
		return luaL_error(L, "pack word %d", n);
	}
	uint8_t buf[2] = {
		(uint8_t)n&0xff ,
		(uint8_t)((n>>8) & 0xff) ,
	};
	lua_pushlstring(L, (char *)buf, 2);
	return 1;
}

static int
lpackint32(lua_State *L) {
	int32_t sn = (int32_t)readinteger(L, 1);
	uint32_t n = (uint32_t) sn;
	uint8_t buf[4] = {
		(uint8_t)n&0xff ,
		(uint8_t)((n>>8) & 0xff) ,
		(uint8_t)((n>>16) & 0xff) ,
		(uint8_t)((n>>24) & 0xff) ,
	};
	lua_pushlstring(L, (char *)buf, 4);
	return 1;
}

static int
lpackcolor(lua_State *L) {
	uint32_t n = luaL_checkinteger(L,1);

	uint8_t buf[4] = {
		(uint8_t)n&0xff ,
		(uint8_t)((n>>8) & 0xff) ,
		(uint8_t)((n>>16) & 0xff) ,
		(uint8_t)((n>>24) & 0xff) ,
	};
	lua_pushlstring(L, (char *)buf, 4);
	return 1;
}

static int
lpackstring(lua_State *L) {
	size_t sz = 0;
	const char *str = lua_tolstring(L,1,&sz);
	if (sz >= 255) {
		return luaL_error(L, "%s is too long", str);
	}
	if (str == NULL) {
		uint8_t buf[1] = { 255 };
		lua_pushlstring(L, (char *)buf, 1);
	} else {
		ARRAY(uint8_t, buf, sz+1);
		buf[0] = sz;
		memcpy(buf+1, str, sz);
		lua_pushlstring(L, (char *)(uint8_t *)buf, sz + 1);
	}
	return 1;
}

static int
lpackframetag(lua_State *L) {
	const char * tagstr = luaL_checkstring(L,1);
	int i;
	int tag = 0;
	for (i=0;tagstr[i];i++) {
		switch(tagstr[i]) {
		case 'i':
			tag |= TAG_ID;
			break;
		case 'c':
			tag |= TAG_COLOR;
			break;
		case 'a':
			tag |= TAG_ADDITIVE;
			break;
		case 'm':
			tag |= TAG_MATRIX;
			break;
		case 't':
			tag |= TAG_TOUCH;
			break;
		case 'M':
			tag |= TAG_MATRIXREF;
			break;
		case 'C':
			tag |= TAG_COLMAP;
			break;
		default:
			return luaL_error(L, "Invalid tag %s", tagstr);
			break;
		}
	}
	uint8_t buf[1] = { (uint8_t)tag };
	lua_pushlstring(L, (char *)buf, 1);
	return 1;
}

static int
lpicture_size(lua_State *L) {
	int n = (int)luaL_checkinteger(L,1);
	int sz = SIZEOF_PICTURE + n * SIZEOF_QUAD;
	lua_pushinteger(L, sz);
	return 1;
}

static int
lpolygon_size(lua_State *L) {
	int n = (int)luaL_checkinteger(L,1);
	int pn = (int)luaL_checkinteger(L,2);
	int sz = SIZEOF_POLYGON
		+ n * SIZEOF_POLY
		+ 12 * pn;
	lua_pushinteger(L, sz);
	return 1;
}

static int
lpack_size(lua_State *L) {
	int max_id = (int)luaL_checkinteger(L,1);
	int tex = (int)luaL_checkinteger(L,2);
	int align_n = (max_id + 1 + 3) & ~3;
	int size = SIZEOF_PACK
		+ align_n * sizeof(uint8_t)
		+ (max_id+1) * SIZEOF_POINTER
		+ tex * sizeof(int);

	lua_pushinteger(L, size);

	return 1;
}

/*
	integer frame
	integer component
	integer action

	ret: integer size
 */
static int
lanimation_size(lua_State *L) {
	int frame = (int)luaL_checkinteger(L,1);
	int component = (int)luaL_checkinteger(L,2);
	int action = (int)luaL_checkinteger(L,3);

	int size = SIZEOF_ANIMATION
		+ frame * SIZEOF_FRAME
		+ action * SIZEOF_ACTION
		+ component * SIZEOF_COMPONENT;

	lua_pushinteger(L, size);
	return 1;
}

static int
lpart_size(lua_State *L) {
	int size;
	if (lua_istable(L,1)) {
		size = SIZEOF_PART + SIZEOF_MATRIX;
	} else {
		size = SIZEOF_PART;
	}
	lua_pushinteger(L, size);

	return 1;
}

static int
lstring_size(lua_State *L) {
	int size;
	if (lua_isnoneornil(L,1)) {
		size = 0;
	} else {
		size_t sz = 0;
		luaL_checklstring(L,1,&sz);
		size = (sz + 1 + 3) & ~3;
	}
	lua_pushinteger(L, size);
	return 1;
}

static int
llabel_size(lua_State *L) {
	lua_pushinteger(L, SIZEOF_LABEL);
	return 1;
}

static int
lpannel_size(lua_State *L) {
	lua_pushinteger(L, SIZEOF_PANNEL);
	return 1;
}

void
dump_pack(struct sprite_pack *pack) {
	if (pack == NULL)
		return;
	int i;
	for (i=0;i<pack->n;i++) {
		if (pack->type[i] == TYPE_PICTURE)
			printf("%d : PICTURE\n", i);
		else {
			struct pack_animation *ani = (struct pack_animation *)pack->data[i];
			printf("%d : ANIMATION %d\n", i, ani->component_number);
			int i;
			for (i=0;i<ani->component_number;i++) {
				printf("\t%d %s\n",ani->component[i].id, ani->component[i].name ? ani->component[i].name : "");
			}
		}
	}
}

static int
ldumppack(lua_State *L) {
	dump_pack((struct sprite_pack *)lua_touserdata(L, 1));
	return 0;
}
 
//static int
//limport_value(lua_State *L) {
//	size_t sz = 0;
//	const uint8_t * data = ejex_buf_extract(L, 1, &sz);
//	if(!data) {
//		data = (const uint8_t *)luaL_checklstring(L, 1, &sz);
//	}
//	int off = luaL_checkinteger(L, 2) - 1;
//	int len = 0;
//	const char * type = luaL_checkstring(L,3);
//	switch(type[0]) {
//	case 'w': {
//		len = 2;
//		if (off+len > sz) {
//			return luaL_error(L, "Invalid data stream");
//		}
//		int v = data[off] | data[off+1] << 8;
//		lua_pushinteger(L, v);
//		break;
//	}
//	case 'i': {
//		len = 4;
//		if (off+len > sz) {
//			return luaL_error(L, "Invalid data stream");
//		}
//		uint32_t v = data[off] 
//			| data[off+1] << 8
//			| data[off+2] << 16
//			| data[off+3] << 24;
//		lua_pushinteger(L, v);
//		break;
//	}
//	case 's': {
//		len = 1;
//		if (off+len > sz) {
//			return luaL_error(L, "Invalid data stream");
//		}
//		int slen = data[off];
//		if (slen == 0xff) {
//			lua_pushlstring(L, "", 0);
//			break;
//		}
//		if (off+len+slen > sz) {
//			return luaL_error(L, "Invalid data stream");
//		}
//		lua_pushlstring(L, (const char *)(data+off+1), slen);
//		len += slen;
//		break;
//	}
//	case 'p' : {
//		len = 0;
//		lua_pushlightuserdata(L, (void *)(data+off));
//		break;
//	}
//	default:
//		return luaL_error(L, "Invalid type %s", type);
//	}
//	lua_pushinteger(L, off + len + 1);
//	return 2;
//}

static int
ldump_bin(lua_State *L) {
	size_t sz;
	const char *bin;
	if(lua_isstring(L, 1)) {
		size_t ssz;
		bin = (const char*)luaL_checklstring(L, 1, &ssz);
		sz = (size_t)luaL_checkinteger(L, 2);
		sz = sz <= ssz ? sz : ssz;
	} else {
		bin = (const char*)lua_touserdata(L, 1);
		if(!bin) {
			return luaL_error(L, "dump_bin need char *");
		}
		sz = (size_t)luaL_checkinteger(L, 2);
	}
	size_t i;
	for(i=0; i<sz; ++i) {
		printf("%02X ", (uint8_t)bin[i]);
		if(i % 16 == 15) printf("\n");
	}
	return 0;
}

int
ejoy2d_spritepack(lua_State *L) {
	luaL_Reg l[] = {
		{ "byte", lpackbyte },
		{ "word", lpackword },
		{ "int32", lpackint32 },
		{ "color", lpackcolor },
		{ "string", lpackstring },
		{ "frametag", lpackframetag },
		{ "picture_size", lpicture_size },
		{ "polygon_size", lpolygon_size },
		{ "pack_size", lpack_size },
		{ "animation_size", lanimation_size },
		{ "part_size", lpart_size },
		{ "string_size" , lstring_size },
		{ "label_size", llabel_size },
		{ "pannel_size", lpannel_size },
		{ "import", limport },
		//{ "import_value", limport_value },
		{ "dump", ldumppack },
		{ "dump_bin", ldump_bin },
		{ NULL, NULL },
	};

	luaL_newlib(L,l);

	lua_pushinteger(L, TYPE_PICTURE);
	lua_setfield(L, -2, "TYPE_PICTURE");
	lua_pushinteger(L, TYPE_ANIMATION);
	lua_setfield(L, -2, "TYPE_ANIMATION");
	lua_pushinteger(L, TYPE_POLYGON);
	lua_setfield(L, -2, "TYPE_POLYGON");
	lua_pushinteger(L, TYPE_LABEL);
	lua_setfield(L, -2, "TYPE_LABEL");
	lua_pushinteger(L, TYPE_PANNEL);
	lua_setfield(L, -2, "TYPE_PANNEL");
	lua_pushinteger(L, TYPE_MATRIX);
	lua_setfield(L, -2, "TYPE_MATRIX");
	lua_pushinteger(L, TYPE_PARTICLE3D);
	lua_setfield(L, -2, "TYPE_PARTICLE3D");
	lua_pushinteger(L, TYPE_PARTICLE2D);
	lua_setfield(L, -2, "TYPE_PARTICLE2D");

	return 1;
}
