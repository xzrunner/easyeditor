#include "rg_dopesheet.h"

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

enum DIMENSION {
	DIM_TRANS_X = 1<<0,
	DIM_TRANS_Y = 1<<1,
	DIM_ROT     = 1<<2,
	DIM_SCALE_X = 1<<3,
	DIM_SCALE_Y = 1<<4,
	DIM_SHEAR_X = 1<<5,
	DIM_SHEAR_Y = 1<<6,
};

enum INTERPOLATION {
	LERP_NULL = 0,
	LERP_LINEAR,
};

struct frame {
	uint16_t time;
	uint8_t	 lerp;
	uint8_t  padding;
	float	 data;
};

struct dimesion {
	uint16_t     frame_count;
	uint16_t     padding;
	struct frame frames[1];
};

struct skin {
	uint16_t time;
	uint16_t skin;
};

struct rg_dopesheet {
	uint8_t	         type;
	uint8_t          dims_count;
	uint16_t         skin_count;
	struct dimesion* dims;
	struct skin      skins[1];
};

static inline bool
_query_dim(const struct dimesion* dim, int time, uint8_t* ptr, float* ret) {
	if (*ptr >= dim->frame_count) {
		return false;
	}

	const struct frame* frame = NULL;
	while (true) {
		frame = &dim->frames[*ptr];
		if (frame->time > time) {
			break;
		}
		++(*ptr);
		if (*ptr >= dim->frame_count) {
			break;
		}
	}

	if (!frame) {
		return false;
	}

	assert(*ptr <= dim->frame_count - 1);
	if (*ptr == dim->frame_count - 1) {
		*ret = dim->frames[*ptr].data;
	} else {
		const struct frame* curr = &dim->frames[*ptr];
		assert(time >= curr->time);
		if (curr->lerp == 0) {
			*ret = curr->data;
		} else {
			// LERP_LINEAR
			const struct frame* next = &dim->frames[*ptr + 1];
			assert(time < next->time);
			*ret = (time - curr->time) * (next->data - curr->data) / (next->time - curr->time) + curr->data;
		}
	}

	return true;
}

void 
rg_ds_query(const struct rg_dopesheet* ds, int time, uint64_t* dims_ptr, struct rg_dopesheet_state* state) {
	memset(state, 0, sizeof(*state));
	int ptr = 0;
	uint64_t old_dims_ptr = *dims_ptr;
	uint64_t new_dims_ptr = 0;
	if (ds->type & DIM_TRANS_X) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr * 8)) & 0xff;
		_query_dim(&ds->dims[ptr], time, &dim_ptr, &state->trans[0]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr;
	}
	if (ds->type & DIM_TRANS_Y) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr * 8)) & 0xff;
		_query_dim(&ds->dims[ptr], time, &dim_ptr, &state->trans[1]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr;
	}
	if (ds->type & DIM_ROT) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr * 8)) & 0xff;
		_query_dim(&ds->dims[ptr], time, &dim_ptr, &state->rot);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr;
	}
	if (ds->type & DIM_SCALE_X) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr * 8)) & 0xff;
		_query_dim(&ds->dims[ptr], time, &dim_ptr, &state->scale[0]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr;
	}
	if (ds->type & DIM_SCALE_Y) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr * 8)) & 0xff;
		_query_dim(&ds->dims[ptr], time, &dim_ptr, &state->scale[1]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr;
	}
	if (ds->type & DIM_SHEAR_X) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr * 8)) & 0xff;
		_query_dim(&ds->dims[ptr], time, &dim_ptr, &state->shear[0]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr;
	}
	if (ds->type & DIM_SHEAR_Y) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr * 8)) & 0xff;
		_query_dim(&ds->dims[ptr], time, &dim_ptr, &state->shear[1]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr;
	}
	*dims_ptr = new_dims_ptr;
}