#include "rg_skeleton_skin.h"
#include "rg_skeleton.h"
#include "rg_dopesheet.h"
#include "rg_slot.h"

void 
rg_skeleton_skin_update(struct rg_skeleton_skin* ss, const struct rg_skeleton* sk, const struct rg_ds_skin** ds, int time) {
	for (int i = 0; i < sk->slot_count; ++i) {
		if (ds[i]) {
			ss->skins[i] = rg_ds_query_skin(ds[i], time);
		} else {
			ss->skins[i] = sk->slots[i].skin;
		}
	}
}