#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_pose_mat_h
#define rigging_pose_mat_h

struct rg_pose_mat {
	float m[6];
};

void rg_pose_mat_identity(struct rg_pose_mat*);

void rg_local2worldmat(const struct rg_pose_mat* src, const struct rg_pose_srt* local, struct rg_pose_mat* dst);

#endif // rigging_pose_mat_h

#ifdef __cplusplus
}
#endif