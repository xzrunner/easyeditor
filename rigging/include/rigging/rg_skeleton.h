#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_rigging_h
#define rigging_rigging_h

struct rg_skeleton {
	int joint_count;
	struct rg_joint** joints;
};

#define SIZEOF_RG_SKELETON (sizeof(struct rg_skeleton) + PTR_SIZE_DIFF)

void rg_skeleton_init(void (*render_func)(void* sym, float x, float y, float angle, float sx, float sy, void* ud));

void rg_skeleton_draw(const struct rg_skeleton*, const void* ud);

#endif // rigging_rigging_h

#ifdef __cplusplus
}
#endif