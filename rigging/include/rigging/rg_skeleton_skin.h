#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_skeleton_skin_h
#define rigging_skeleton_skin_h

#include <stdint.h>

struct rg_skeleton_skin {
	uint16_t skins[1];
};

#define SIZEOF_RG_SKELETON_SKIN (sizeof(struct rg_skeleton_skin) - sizeof(uint16_t))

void rg_skeleton_skin_update(struct rg_skeleton_skin*, const struct rg_skeleton*, const struct rg_ds_skin**, int time);

#endif // rigging_skeleton_skin_h

#ifdef __cplusplus
}
#endif