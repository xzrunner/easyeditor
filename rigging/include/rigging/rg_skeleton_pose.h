#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_skeleton_pose_h
#define rigging_skeleton_pose_h

#include "rg_joint_pose.h"

struct rg_pose_pair {
	struct rg_joint_pose local;
	struct rg_joint_pose world;
};

struct rg_skeleton_pose {
	struct rg_pose_pair poses[1];
};

#define SIZEOF_RG_SKELETON_POSE (sizeof(struct rg_skeleton_pose) - sizeof(struct rg_pose_pair))

void rg_skeleton_pose_update(struct rg_skeleton_pose*, struct rg_skeleton*, int joint_idx);

#endif // rigging_skeleton_pose_h

#ifdef __cplusplus
}
#endif