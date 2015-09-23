#ifndef EJOY_2D_SPRITE_H
#define EJOY_2D_SPRITE_H

#include "spritepack.h"
#include "matrix.h"

#include <lua.h>
#include <stdint.h>
#include <stdbool.h>

#define SPRFLAG_INVISIBLE               (0x00000001)
#define SPRFLAG_MESSAGE                 (0x00000002)
#define SPRFLAG_MULTIMOUNT              (0x00000004)
#define SPRFLAG_FORCE_INHERIT_FRAME     (0x00000008)
#define SPRFLAG_RANDOM_CHILD_BASE_FRAME (0x00000010)

struct material;

struct anchor_data {
	struct particle_system *ps;
	struct pack_picture *pic;
	struct matrix mat;
	struct ej_trail_t *trail;
};

struct sprite {
	struct sprite * parent;
	uint16_t type;
	uint16_t id;
	struct sprite_trans t;
	union {
		struct pack_animation *ani;
		struct pack_picture *pic;
		struct pack_polygon *poly;
		struct pack_label *label;
		struct pack_pannel *pannel;
		struct matrix *mat;
		struct pack_particle3d *p3d;
		struct pack_particle2d *p2d;
	} s;
	struct matrix mat;
	int start_frame;
	int total_frame;
	int frame;
	int flags;
	const char *name;	// name for parent
	struct material *material;
	union {
		struct sprite * children[1];
		struct rich_text * rich_text;
		int scissor;
		struct anchor_data *anchor;
	} data;
};

// sprite_size must be call before sprite_init
int sprite_size(struct sprite_pack *pack, int id);
void sprite_init(struct sprite *, struct sprite_pack * pack, int id, int sz);

int sprite_setframe(struct sprite *, int frame, bool force_child);

struct sprite_trans * sprite_trans_mul(struct sprite_trans *a, struct sprite_trans *b, struct sprite_trans *t, struct matrix *tmp_matrix);

#endif
