#include "rg_skeleton.h"
#include "rg_joint.h"
#include "rg_skeleton_pose.h"

#include <stdlib.h>
#include <assert.h>

static void (*RENDER_FUNC)(void* sym, float x, float y, float angle, float sx, float sy, const void* ud);

void 
rg_skeleton_init(void (*render_func)(void* sym, float x, float y, float angle, float sx, float sy, const void* ud)) {
	RENDER_FUNC = render_func;
}

void 
rg_skeleton_draw(const struct rg_skeleton* sk, const struct rg_skeleton_pose* pose, const void* ud) {
	for (int i = 0; i < sk->joint_count; ++i) {
		int skin_idx = 0xffff;
		if (pose->poses[i].skin != 0xffff) {
			skin_idx = pose->poses[i].skin;
		} else {
			const struct rg_joint* joint = sk->joints[i];
			if (joint->skin >= 0 && joint->skin < sk->skin_count) {
				skin_idx = joint->skin;
			}
		}
		if (skin_idx == 0xffff) {
			continue;
		}

		const struct rg_skin* skin = &sk->skins[skin_idx];
		assert(skin->ud);

		struct rg_joint_pose world;
		rg_local2world(&pose->poses[i].world, &skin->local, &world);

		RENDER_FUNC(skin->ud, world.trans[0], world.trans[1], world.rot, world.scale[0], world.scale[1], ud);
	}
}