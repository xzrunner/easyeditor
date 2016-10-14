#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_dopesheet_h
#define rigging_dopesheet_h

#include <stdint.h>

struct rg_dopesheet;

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