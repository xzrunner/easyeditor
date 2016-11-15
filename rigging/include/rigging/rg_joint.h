#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_joint_h
#define rigging_joint_h

#include "rg_pose_mat.h"
#include "rg_pose_srt.h"
#include "rg_utility.h"

#include <stdint.h>

struct rg_joint {
	const char* name;

	struct rg_pose_mat world_pose;
	struct rg_pose_srt local_pose;

	uint8_t  parent;

	uint8_t  children_count;
	uint8_t  children[1];
};

#define SIZEOF_RG_JOINT (sizeof(struct rg_joint) - sizeof(uint8_t) + PTR_SIZE_DIFF)

void rg_joint_update(struct rg_joint*, struct rg_skeleton*);

#endif // rigging_joint_h

#ifdef __cplusplus
}
#endif