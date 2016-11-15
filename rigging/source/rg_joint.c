#include "rg_joint.h"
#include "rg_pose_srt.h"
#include "rg_skeleton.h"

#include <stdint.h>
#include <assert.h>

void 
rg_joint_update(struct rg_joint* joint, struct rg_skeleton* sk) {
	if (joint->parent != 0xff) {
		assert(joint->parent < sk->joint_count);
		struct rg_joint* parent = sk->joints[joint->parent];
		rg_local2world(&parent->world_pose, &joint->local_pose, &joint->world_pose);
	}
	for (int i = 0; i < joint->children_count; ++i) {
		assert(joint->children[i] < sk->joint_count);
		struct rg_joint* child = sk->joints[joint->children[i]];
		rg_joint_update(child, sk);
	}
}