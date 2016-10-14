#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_rigging_h
#define rigging_rigging_h

struct rg_skeleton;

void rg_skeleton_init(void (*render_func)(void* sym, float x, float y, float angle, float sx, float sy));

struct rg_skeleton* rg_skeleton_create(struct rg_joint** joints, int count);
void rg_skeleton_release(struct rg_skeleton*);

void rg_skeleton_draw(const struct rg_skeleton*, const void* ud);

#endif // rigging_rigging_h

#ifdef __cplusplus
}
#endif