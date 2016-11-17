#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_skeleton_pose_h
#define rigging_skeleton_pose_h

#include "rg_pose_srt.h"
#include "rg_pose_mat.h"

#include <stdint.h>

struct rg_skeleton;
struct rg_tl_joint;

struct rg_pose_pair {
	struct rg_pose_srt local;
	struct rg_pose_mat world;
};

struct rg_skeleton_pose {
	struct rg_pose_pair poses[1];
};

#define SIZEOF_RG_SKELETON_POSE (sizeof(struct rg_skeleton_pose) - sizeof(struct rg_pose_pair))

void rg_skeleton_pose_update(struct rg_skeleton_pose*, const struct rg_skeleton*, const struct rg_tl_joint**, int time);

#endif // rigging_skeleton_pose_h

#ifdef __cplusplus
}
#endif