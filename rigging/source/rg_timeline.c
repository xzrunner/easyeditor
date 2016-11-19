#include "rg_timeline.h"
#include "rg_utility.h"

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define MESH_BUF_SIZE 1024

static float* MESH_BUF = NULL;

void 
rg_timeline_init() {
	int sz = sizeof(float) * MESH_BUF_SIZE;
	MESH_BUF = malloc(sz);
	memset(MESH_BUF, 0, sz);
}

// static inline bool
// _query_dim(const struct rg_frame* samples, int sample_count, int time, uint8_t* ptr, float* ret) {
// 	if (*ptr >= sample_count) {
// 		return false;
// 	}
// 
// 	const struct rg_frame* frame = NULL;
// 	while (true) {
// 		frame = &samples[*ptr];
// 		if (frame->time > time) {
// 			break;
// 		}
// 		++(*ptr);
// 		if (*ptr >= sample_count) {
// 			break;
// 		}
// 	}
// 
// 	if (!frame) {
// 		return false;
// 	}
// 
// 	assert(*ptr <= sample_count - 1);
// 	if (*ptr == sample_count - 1) {
// 		*ret = samples[*ptr].data;
// 	} else {
// 		const struct rg_frame* curr = &samples[*ptr];
// 		assert(time >= curr->time);
// 		if (curr->lerp == 0) {
// 			*ret = curr->data;
// 		} else {
// 			// LERP_LINEAR
// 			const struct rg_frame* next = &samples[*ptr + 1];
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

static inline float
_float_lerp(uint16_t time_begin, uint16_t time_end, float begin, float end, uint16_t time) {
	return (time - time_begin) * (end - begin) / (time_end - time_begin) + begin;
}

static inline bool
_query_joint(const struct rg_joint_sample* samples, int sample_count, int time, bool is_angle, uint8_t* ptr, float* ret) {
	assert(sample_count > 0);

	if (time < samples[0].time) {
		*ptr = 0;
		return false;
	}
	if (sample_count == 1 || time > samples[sample_count - 1].time) {
		*ptr = 0;
		*ret = samples[sample_count - 1].data;
		return true;
	}

	*ptr = 0;

	int curr = 0, next = 1;
	while (true)
	{
		if (samples[curr].time <= time && time <= samples[next].time) {
			const struct rg_joint_sample* c = &samples[curr];
			const struct rg_joint_sample* n = &samples[next];
			float cd, nd;
			if (is_angle) {
				cd = _format_angle(c->data);
				nd = _format_angle(n->data);
			} else {
				cd = c->data;
				nd = n->data;
			}
			*ret = _float_lerp(c->time, n->time, cd, nd, time);
			return true;
		} else {
			++curr;
			++next;
			if (next >= sample_count) {
				break;
			}
		}
	}

	return false;
}

void 
rg_tl_query_joint(const struct rg_tl_joint* joint, int time, uint64_t* dims_ptr, struct rg_tl_joint_state* state) {
	memset(state, 0, sizeof(*state));
	state->scale[0] = state->scale[1] = 1;

	int ptr_dims = 0, ptr_sample = 0;
	uint64_t old_dims_ptr = *dims_ptr;
	uint64_t new_dims_ptr = 0;
	if (joint->type & DIM_FLAG_TRANS_X) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&joint->samples[ptr_sample], joint->dims_count[DIM_IDX_TRANS_X], time, false, &dim_ptr, &state->trans[0]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_sample += joint->dims_count[DIM_IDX_TRANS_X];
	}
	if (joint->type & DIM_FLAG_TRANS_Y) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&joint->samples[ptr_sample], joint->dims_count[DIM_IDX_TRANS_Y], time, false, &dim_ptr, &state->trans[1]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_sample += joint->dims_count[DIM_IDX_TRANS_Y];
	}
	if (joint->type & DIM_FLAG_ROT) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&joint->samples[ptr_sample], joint->dims_count[DIM_IDX_ROT], time, true, &dim_ptr, &state->rot);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_sample += joint->dims_count[DIM_IDX_ROT];
	}
	if (joint->type & DIM_FLAG_SCALE_X) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&joint->samples[ptr_sample], joint->dims_count[DIM_IDX_SCALE_X], time, false, &dim_ptr, &state->scale[0]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_sample += joint->dims_count[DIM_IDX_SCALE_X];
	}
	if (joint->type & DIM_FLAG_SCALE_Y) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&joint->samples[ptr_sample], joint->dims_count[DIM_IDX_SCALE_Y], time, false, &dim_ptr, &state->scale[1]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_sample += joint->dims_count[DIM_IDX_SCALE_Y];
	}
	if (joint->type & DIM_FLAG_SHEAR_X) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&joint->samples[ptr_sample], joint->dims_count[DIM_IDX_SHEAR_X], time, false, &dim_ptr, &state->shear[0]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_sample += joint->dims_count[DIM_IDX_SHEAR_X];
	}
	if (joint->type & DIM_FLAG_SHEAR_Y) {
		uint8_t dim_ptr = (old_dims_ptr >> (ptr_dims * 8)) & 0xff;
		_query_joint(&joint->samples[ptr_sample], joint->dims_count[DIM_IDX_SHEAR_Y], time, false, &dim_ptr, &state->shear[1]);
		new_dims_ptr = (new_dims_ptr << 8) & dim_ptr;
		++ptr_dims;
		ptr_sample += joint->dims_count[DIM_IDX_SHEAR_Y];
	}
	*dims_ptr = new_dims_ptr;
}

uint16_t 
rg_tl_query_skin(const struct rg_tl_skin* skin, int time) {
	assert(skin && skin->skins && skin->skin_count > 0);

	if (time < skin->skins[0].time) {
		return RG_SKIN_UNKNOWN;
	}
	if (skin->skin_count == 1 || time >= skin->skins[skin->skin_count - 1].time) {
		return skin->skins[skin->skin_count - 1].skin;
	}

	int curr = 0, next = 1;
	while (true)
	{
		if (skin->skins[curr].time <= time && time < skin->skins[next].time) {
			return skin->skins[curr].skin;
		} else {
			++curr;
			++next;
			if (next >= skin->skin_count) {
				break;
			}
		}
	}

	return RG_SKIN_UNKNOWN;
}

static void 
_query_deform(const struct rg_tl_deform* deform, int time, const struct rg_deform_sample** curr, const struct rg_deform_sample** next) {
	*curr = NULL;
	*next = NULL;
	if (time < deform->samples[0].time) {
		return;
	}
	if (deform->count == 1 || time > deform->samples[deform->count - 1].time) {
		*curr = &deform->samples[deform->count - 1];
		return;
	}

	int curr_ptr = 0, next_ptr = 1;
	while (true)
	{
		if (deform->samples[curr_ptr].time <= time && time < deform->samples[next_ptr].time) {
			*curr = &deform->samples[curr_ptr];
			*next = &deform->samples[next_ptr];
			return;
		} else {
			++curr_ptr;
			++next_ptr;
			if (next_ptr >= deform->count) {
				break;
			}
		}
	}
}

const float* 
rg_tl_query_deform(const struct rg_tl_deform* deform, int time, struct rg_tl_deform_state* state) {
	state->offset0 = 0;
	state->count0  = 0;
	state->offset1 = 0;
	state->count1  = 0;

	const struct rg_deform_sample* curr;
	const struct rg_deform_sample* next;
	_query_deform(deform, time, &curr, &next);
	if (!curr && !next) {
		return NULL;
	}

	int count = 0;
	if (curr) {
		count += curr->count;
	}
	if (next) {
		count += next->count;
	}
	assert(count < MESH_BUF_SIZE * 2);

	int buf_ptr = 0;
	if (curr && next) {
		int ptr = 0;
		for (int i = 0; i < curr->count; ++i) {
			MESH_BUF[buf_ptr++] = _float_lerp(curr->time, next->time, curr->data[ptr++], 0, time);
			MESH_BUF[buf_ptr++] = _float_lerp(curr->time, next->time, curr->data[ptr++], 0, time);
		}
		ptr = 0;
		for (int i = 0; i < next->count; ++i) {
			MESH_BUF[buf_ptr++] = _float_lerp(curr->time, next->time, 0, next->data[ptr++], time);
			MESH_BUF[buf_ptr++] = _float_lerp(curr->time, next->time, 0, next->data[ptr++], time);
		}
	} else if (curr) {
		memcpy(MESH_BUF, curr->data, curr->count * 2);
	} else {
		assert(next);
		memcpy(MESH_BUF, next->data, next->count * 2);
	}

	if (curr) {
		state->offset0 = curr->offset;
		state->count0  = curr->count;
	}
	if (next) {
		state->offset1 = next->offset;
		state->count1  = next->count;
	}

	return MESH_BUF;
}