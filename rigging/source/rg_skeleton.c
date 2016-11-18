#include "rg_skeleton.h"
#include "rg_joint.h"
#include "rg_skeleton_pose.h"
#include "rg_skeleton_skin.h"
#include "rg_slot.h"

#include <stdlib.h>
#include <assert.h>

#include <string.h>

static void (*RENDER_FUNC)(void* sym, float* mat, const void* ud);

void 
rg_skeleton_init(void (*render_func)(void* sym, float* mat, const void* ud)) {
	RENDER_FUNC = render_func;
}

void 
rg_skeleton_draw(const struct rg_skeleton* sk, const struct rg_skeleton_pose* pose, const struct rg_skeleton_skin* ss, const void* ud) {
	for (int i = 0; i < sk->slot_count; ++i) {
		const struct rg_slot* slot = &sk->slots[i];
		uint16_t skin_idx = RG_SKIN_UNKNOWN;
		if (ss->skins[i]) {
			skin_idx = ss->skins[i];
		} 
		if (skin_idx == RG_SKIN_UNKNOWN) {
			skin_idx = slot->skin;
		}
		if (skin_idx == RG_SKIN_UNKNOWN || skin_idx == RG_SKIN_NULL) {
			continue;
		}

		const struct rg_skin* skin = &sk->skins[skin_idx];
		assert(skin->ud);

		struct rg_pose_mat world;
		if (skin->type != SKIN_JOINT_MESH) {
			rg_local2worldmat(&pose->poses[slot->joint].world, &skin->local, &world);
		} else {
			rg_pose_mat_identity(&world);
		}

		RENDER_FUNC(skin->ud, world.m, ud);
	}
}