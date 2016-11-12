#include "rg_skeleton_pose.h"
#include "rg_skeleton.h"
#include "rg_joint.h"

#include <assert.h>

void 
rg_skeleton_pose_update(struct rg_skeleton_pose* pose, struct rg_skeleton* sk, int joint_idx) {
	assert(joint_idx >= 0 && joint_idx < sk->joint_count);
	struct rg_joint* joint = sk->joints[joint_idx];
	if (joint->parent != 0xff) {
		assert(joint->parent < sk->joint_count);
		rg_local2world(&pose->poses[joint->parent].world, &pose->poses[joint_idx].local, &pose->poses[joint_idx].world);
	}
	for (int i = 0; i < joint->children_count; ++i) {
		rg_skeleton_pose_update(pose, sk, joint->children[i]);
	}
}