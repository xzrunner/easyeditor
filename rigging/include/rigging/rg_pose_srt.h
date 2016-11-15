#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_pose_srt_h
#define rigging_pose_srt_h

struct rg_pose_srt {
	float trans[2];
	float rot;
	float scale[2];
};

void rg_pose_srt_lerp(const struct rg_pose_srt* begin, const struct rg_pose_srt* end, float process, struct rg_pose_srt* dst);

void rg_pose_srt_identity(struct rg_pose_srt*);

void rg_pose_srt_inv(const struct rg_pose_srt* src, struct rg_pose_srt* dst);

void rg_local2world(const struct rg_pose_srt* src, const struct rg_pose_srt* local, struct rg_pose_srt* dst);
void rg_world2local(const struct rg_pose_srt* src, const struct rg_pose_srt* dst, struct rg_pose_srt* local);

#endif // rigging_pose_srt_h

#ifdef __cplusplus
}
#endif