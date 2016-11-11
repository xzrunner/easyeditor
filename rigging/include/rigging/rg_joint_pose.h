#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_joint_pose_h
#define rigging_joint_pose_h

struct rg_joint_pose {
	float trans[2];
	float rot;
	float scale[2];
};

void rg_joint_pose_lerp(const struct rg_joint_pose* begin, const struct rg_joint_pose* end, float process, struct rg_joint_pose* dst);

void rg_joint_pose_identity(struct rg_joint_pose*);

void rg_joint_pose_inv(struct rg_joint_pose* dst, const struct rg_joint_pose* src);

void rg_local2world(const struct rg_joint_pose* src, const struct rg_joint_pose* local, struct rg_joint_pose* dst);
void rg_world2local(const struct rg_joint_pose* src, const struct rg_joint_pose* dst, struct rg_joint_pose* local);

#endif // rigging_joint_pose_h

#ifdef __cplusplus
}
#endif