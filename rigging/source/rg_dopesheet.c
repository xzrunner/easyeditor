#include "rg_dopesheet.h"
#include "rg_utility.h"

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// static inline bool
// _query_dim(const struct rg_frame* frames, int frame_count, int time, uint8_t* ptr, float* ret) {
// 	if (*ptr >= frame_count) {
// 		return false;
// 	}
// 
// 	const struct rg_frame* frame = NULL;
// 	while (true) {
// 		frame = &frames[*ptr];
// 		if (frame->time > time) {
// 			break;
// 		}
// 		++(*ptr);
// 		if (*ptr >= frame_count) {
// 			break;
// 		}
// 	}
// 
// 	if (!frame) {
// 		return false;
// 	}
// 
// 	assert(*ptr <= frame_count - 1);
// 	if (*ptr == frame_count - 1) {
// 		*ret = frames[*ptr].data;
// 	} else {
// 		const struct rg_frame* curr = &frames[*ptr];
// 		assert(time >= curr->time);
// 		if (curr->lerp == 0) {
// 			*ret = curr->data;
// 		} else {
// 			// LERP_LINEAR
// 			const struct rg_frame* next = &frames[*ptr + 1];
// 			assert(time < next->time);
// 			*ret = (time - curr->time) * (next->data - curr->data) / (next->time - curr->time) + curr->data;
// 		}
// 	}
// 
// 	return true;
// }

static inline float
_format_angle(float angle) {
	const float PI = 3.1415926f;
	if (angle > PI) {
		angle -= PI * 2;
	}
	if (angle < -PI) {
		angle += PI * 2;
	}
	return angle;
}

static inline bool
_query_joint(const struct rg_ds_joint_frame* frames, int frame_count, int time, bool is_angle, uint8_t* ptr, float* ret) {
	assert(frame_count > 0);

	if (time < frames[0].time) {
		*ptr = 0;
		return false;
	}
	if (frame_count == 1 || time > frames[frame_count - 1].time) {
		*ptr = 0;
		*ret = frames[frame_count - 1].data;
		return true;
	}

	*ptr = 0;

	int curr = 0, next = 1;
	while (true)
	{
		if (frames[curr].time <= time && time <= frames[next].time) {
			const struct rg_ds_joint_frame* c = &frames[curr];
			const struct rg_ds_joint_frame* n = &frames[next];
			float cd, nd;
			if (is_angle) {
				cd = _format_angle(c->data);
				nd = _format_angle(n->data);
			} else {
				cd = c->data;
				nd = n->data;
			}
			*ret = (time - c->time) * (nd - cd) / (n->time - c->time) + cd;
			return true;
		} else {
			++curr;
			++next;
			if (next >= frame_count) {
				break;
			}
		}
	}

	return false;
}

void 
rg_ds_query_joint(const struct rg_ds_joint* ds, int time, uint64_t* dims_ptr, struct rg_ds_joint_state* state) {
	memset(state, 0, sizeof(*state));
	state->scale[0] = state->scale[1] = 1;

	int ptr_dims = 0, ptr_frame = 0;
	uint64_t old_dims_ptr = *dims_ptr;
	uint64_t new_dims_ptr = 0;
	if (ds->type & DIM_FLAG_TRANS_X) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&ds->frames[ptr_frame], ds->dims_count[DIM_IDX_TRANS_X], time, false, &dim_ptr, &state->trans[0]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_frame += ds->dims_count[DIM_IDX_TRANS_X];
	}
	if (ds->type & DIM_FLAG_TRANS_Y) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&ds->frames[ptr_frame], ds->dims_count[DIM_IDX_TRANS_Y], time, false, &dim_ptr, &state->trans[1]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_frame += ds->dims_count[DIM_IDX_TRANS_Y];
	}
	if (ds->type & DIM_FLAG_ROT) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&ds->frames[ptr_frame], ds->dims_count[DIM_IDX_ROT], time, true, &dim_ptr, &state->rot);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_frame += ds->dims_count[DIM_IDX_ROT];
	}
	if (ds->type & DIM_FLAG_SCALE_X) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&ds->frames[ptr_frame], ds->dims_count[DIM_IDX_SCALE_X], time, false, &dim_ptr, &state->scale[0]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_frame += ds->dims_count[DIM_IDX_SCALE_X];
	}
	if (ds->type & DIM_FLAG_SCALE_Y) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&ds->frames[ptr_frame], ds->dims_count[DIM_IDX_SCALE_Y], time, false, &dim_ptr, &state->scale[1]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_frame += ds->dims_count[DIM_IDX_SCALE_Y];
	}
	if (ds->type & DIM_FLAG_SHEAR_X) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&ds->frames[ptr_frame], ds->dims_count[DIM_IDX_SHEAR_X], time, false, &dim_ptr, &state->shear[0]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_frame += ds->dims_count[DIM_IDX_SHEAR_X];
	}
	if (ds->type & DIM_FLAG_SHEAR_Y) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&ds->frames[ptr_frame], ds->dims_count[DIM_IDX_SHEAR_Y], time, false, &dim_ptr, &state->shear[1]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_frame += ds->dims_count[DIM_IDX_SHEAR_Y];
	}
	*dims_ptr = new_dims_ptr;
}

uint16_t 
rg_ds_query_skin(const struct rg_ds_skin* ds, int time) {
	assert(ds && ds->skins && ds->skin_count > 0);

	if (time < ds->skins[0].time) {
		return RG_SKIN_UNKNOWN;
	}
	if (ds->skin_count == 1 || time >= ds->skins[ds->skin_count - 1].time) {
		return ds->skins[ds->skin_count - 1].skin;
	}

	int curr = 0, next = 1;
	while (true)
	{
		if (ds->skins[curr].time <= time && time < ds->skins[next].time) {
			return ds->skins[curr].skin;
		} else {
			++curr;
			++next;
			if (next >= ds->skin_count) {
				break;
			}
		}
	}

	return RG_SKIN_UNKNOWN;
}