#include "rg_skeleton_pose.h"
#include "rg_skeleton.h"
#include "rg_joint.h"
#include "rg_dopesheet.h"

#include <assert.h>

static void 
_update_joint(struct rg_skeleton_pose* pose, const struct rg_skeleton* sk, int joint_idx) {
	assert(joint_idx >= 0 && joint_idx < sk->joint_count);
	struct rg_joint* joint = sk->joints[joint_idx];
	if (joint->parent != 0xff) {
		assert(joint->parent < sk->joint_count);
		rg_local2worldmat(&pose->poses[joint->parent].world, &pose->poses[joint_idx].local, &pose->poses[joint_idx].world);
	}
	for (int i = 0; i < joint->children_count; ++i) {
		_update_joint(pose, sk, joint->children[i]);
	}
}

void 
rg_skeleton_pose_update(struct rg_skeleton_pose* pose, const struct rg_skeleton* sk, const struct rg_dopesheet** ds, int time) {
	uint64_t dims_ptr = 0;
	for (int i = 0; i < sk->joint_count; ++i) {
		struct rg_joint* joint = sk->joints[i];
		if (ds[i]) {
			rg_pose_srt_identity(&pose->poses[i].local);
			rg_pose_mat_identity(&pose->poses[i].world);

			struct rg_dopesheet_state state;
			rg_ds_query(ds[i], time, &dims_ptr, &state);

			pose->poses[i].local.trans[0] = joint->local_pose.trans[0] + state.trans[0];
			pose->poses[i].local.trans[1] = joint->local_pose.trans[1] + state.trans[1];
			pose->poses[i].local.rot      = joint->local_pose.rot + state.rot;
			pose->poses[i].local.scale[0] = joint->local_pose.scale[0] * state.scale[0];
			pose->poses[i].local.scale[1] = joint->local_pose.scale[1] * state.scale[1];

			pose->poses[i].skin = state.skin;
		} else {
			pose->poses[i].local = joint->local_pose;
			pose->poses[i].skin = 0xffff;
		}
	}

	struct rg_pose_pair* root = &pose->poses[sk->root];
	rg_pose_mat_build(&root->world, &root->local);

	_update_joint(pose, sk, sk->root);
}