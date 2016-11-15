#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_skin_h
#define rigging_skin_h

#include "rg_pose_srt.h"

struct rg_skin {
	struct rg_pose_srt local;
	void* ud;
};

#define SIZEOF_RG_SKIN (sizeof(struct rg_skin) + PTR_SIZE_DIFF * 2)

#endif // rigging_skin_h

#ifdef __cplusplus
}
#endif