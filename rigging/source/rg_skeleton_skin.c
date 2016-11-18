#include "rg_skeleton_skin.h"
#include "rg_skeleton.h"
#include "rg_timeline.h"
#include "rg_slot.h"
#include "rg_utility.h"

static void (*UPDATE_SKIN_FUNC)(void* sym, const struct rg_skeleton_pose*);

void 
rg_skeleton_skin_init(void (*update_skin_func)(void* sym, const struct rg_skeleton_pose*)) {
	UPDATE_SKIN_FUNC = update_skin_func;
}

void 
rg_skeleton_skin_update(struct rg_skeleton_skin* ss, const struct rg_skeleton* sk, const struct rg_skeleton_pose* sk_pose, const struct rg_tl_skin** ts, int time) {
	for (int i = 0; i < sk->slot_count; ++i) {
		uint16_t skin = RG_SKIN_UNKNOWN;
		if (ts[i]) {
			skin = rg_tl_query_skin(ts[i], time);
		} else {
			skin = sk->slots[i].skin;
		}
		ss->skins[i] = skin;

		if (skin != RG_SKIN_UNKNOWN && skin != RG_SKIN_NULL) {
			UPDATE_SKIN_FUNC(sk->skins[skin].ud, sk_pose);
		}
	}
}