#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_skin_h
#define rigging_skin_h

#include "rg_pose_srt.h"

enum RG_SKIN {
	SKIN_IMG,
	SKIN_MESH,
	SKIN_JOINT_MESH
};

struct rg_skin {
	int type;
	struct rg_pose_srt local;
	void* ud;
};

#define SIZEOF_RG_SKIN (sizeof(struct rg_skin) + PTR_SIZE_DIFF)

#endif // rigging_skin_h

#ifdef __cplusplus
}
#endif