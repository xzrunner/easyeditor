#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_dopesheet_h
#define rigging_dopesheet_h

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

struct rg_frame {
	uint16_t time;
	uint8_t	 lerp;
	uint8_t  padding;
	float	 data;
};

struct rg_frame_skin {
	uint16_t time;
	uint16_t skin;
};

struct rg_dopesheet {
	struct rg_frame_skin* skins;
	uint8_t               skin_count;

	uint8_t	        type;
	uint8_t         dims_count[DIM_COUNT];
	uint8_t         padding[3];
	struct rg_frame frames[1];
};

#define SIZEOF_RG_DOPESHEET (sizeof(struct rg_dopesheet) - sizeof(struct rg_frame) + PTR_SIZE_DIFF)

struct rg_dopesheet_state {
	float    trans[2];
	float    rot;
	float    scale[2];
	float    shear[2];
	uint16_t skin;
};

void rg_ds_query(const struct rg_dopesheet*, int time, uint64_t* dims_ptr, struct rg_dopesheet_state*);

#endif // rigging_dopesheet_h

#ifdef __cplusplus
}
#endif