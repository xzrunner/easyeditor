#include "rg_skeleton.h"
#include "rg_joint.h"

#include <stdlib.h>
#include <assert.h>

static void (*RENDER_FUNC)(void* sym, float x, float y, float angle, float sx, float sy, const void* ud);

void 
rg_skeleton_init(void (*render_func)(void* sym, float x, float y, float angle, float sx, float sy, const void* ud)) {
	RENDER_FUNC = render_func;
}

// struct rg_skeleton* 
// rg_skeleton_create(struct rg_joint** joints, int count) {
// 	struct rg_skeleton* sk = malloc(sizeof(*sk));
// 	if (!sk) {
// 		return NULL;
// 	}
// 	sk->joint_count = count;
// 	sk->joints = joints;
// 	return sk;
// }
// 
// void 
// rg_skeleton_release(struct rg_skeleton* sk) {
// 	free(sk);
// }

void 
rg_skeleton_draw(const struct rg_skeleton* sk, const void* ud) {
	for (int i = 0; i < sk->joint_count; ++i) {
		const struct rg_joint* joint = sk->joints[i];
		if (joint->skin == 0xffff) {
			continue;
		}
		assert(joint->skin >= 0 && joint->skin < sk->skin_count);
		const struct rg_skin* skin = &sk->skins[joint->skin];
		if (!skin->ud) {
			continue;
		}

		struct rg_joint_pose world;
		rg_local2world(&joint->world_pose, &skin->local, &world);

		RENDER_FUNC(skin->ud, world.trans[0], world.trans[1], world.rot, world.scale[0], world.scale[1], ud);
	}
}