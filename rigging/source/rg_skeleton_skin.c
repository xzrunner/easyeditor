#include "rg_skeleton_skin.h"
#include "rg_skeleton.h"
#include "rg_timeline.h"
#include "rg_slot.h"

void 
rg_skeleton_skin_update(struct rg_skeleton_skin* ss, const struct rg_skeleton* sk, const struct rg_tl_skin** ts, int time) {
	for (int i = 0; i < sk->slot_count; ++i) {
		if (ts[i]) {
			ss->skins[i] = rg_tl_query_skin(ts[i], time);
		} else {
			ss->skins[i] = sk->slots[i].skin;
		}
	}
}