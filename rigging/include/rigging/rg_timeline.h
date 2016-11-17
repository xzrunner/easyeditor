#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_timeline_h
#define rigging_timeline_h

#include <stdint.h>

enum DIM_IDX {
	DIM_IDX_TRANS_X = 0,
	DIM_IDX_TRANS_Y,
	DIM_IDX_ROT,
	DIM_IDX_SCALE_X,
	DIM_IDX_SCALE_Y,
	DIM_IDX_SHEAR_X,
	DIM_IDX_SHEAR_Y,

	DIM_COUNT
};

enum DIM_FLAG {
	DIM_FLAG_TRANS_X = 1<<0,
	DIM_FLAG_TRANS_Y = 1<<1,
	DIM_FLAG_ROT     = 1<<2,
	DIM_FLAG_SCALE_X = 1<<3,
	DIM_FLAG_SCALE_Y = 1<<4,
	DIM_FLAG_SHEAR_X = 1<<5,
	DIM_FLAG_SHEAR_Y = 1<<6,
};

enum INTERPOLATION {
	LERP_NULL = 0,
	LERP_LINEAR,
};

struct rg_joint_sample {
	uint16_t time;
	uint8_t	 lerp;
	uint8_t  padding;
	float	 data;
};

struct rg_tl_joint_state {
	float    trans[2];
	float    rot;
	float    scale[2];
	float    shear[2];
};

struct rg_tl_joint {
	uint8_t	               type;
	uint8_t                dims_count[DIM_COUNT];
	uint8_t                padding[3];
	struct rg_joint_sample samples[1];
};

#define SIZEOF_RG_TIMELINE_JOINT (sizeof(struct rg_tl_joint) - sizeof(struct rg_joint_sample))

struct rg_skin_sample {
	uint16_t time;
	uint16_t skin;
};

struct rg_tl_skin {
	uint8_t               skin_count;
	struct rg_skin_sample skins[1];
};

#define SIZEOF_RG_TIMELINE_SKIN (sizeof(struct rg_tl_skin) - sizeof(struct rg_skin_sample))

struct rg_timeline {
	struct rg_tl_joint** joints;
	struct rg_tl_skin**  skins;
};

void rg_tl_query_joint(const struct rg_tl_joint*, int time, uint64_t* dims_ptr, struct rg_tl_joint_state*);

uint16_t rg_tl_query_skin(const struct rg_tl_skin*, int time);

#endif // rigging_timeline_h

#ifdef __cplusplus
}
#endif