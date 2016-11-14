#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_skeleton_h
#define rigging_skeleton_h

#include "rg_skin.h"

struct rg_skeleton_pose;

struct rg_skeleton {
	int joint_count;
	struct rg_joint** joints;

	int root;

	int skin_count;
	struct rg_skin skins[1];
};

#define SIZEOF_RG_SKELETON (sizeof(struct rg_skeleton) - sizeof(struct rg_skin) + PTR_SIZE_DIFF)

void rg_skeleton_init(void (*render_func)(void* sym, float x, float y, float angle, float sx, float sy, const void* ud));

void rg_skeleton_draw(const struct rg_skeleton*, const struct rg_skeleton_pose*, const void* ud);

#endif // rigging_skeleton_h

#ifdef __cplusplus
}
#endif