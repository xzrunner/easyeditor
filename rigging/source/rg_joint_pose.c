#include "rg_joint_pose.h"

#include <math.h>

static inline void
_rotate_vector(float* src, float rad, float* dst) {
	float s = sin(rad), c = cos(rad);
 	dst[0] = src[0] * c - src[1] * s;
 	dst[1] = src[0] * s + src[1] * c;
}

void
rg_joint_pose_lerp(const struct rg_joint_pose* begin, const struct rg_joint_pose* end, float process, struct rg_joint_pose* dst) {
	dst->trans[0]	= begin->trans[0] + (end->trans[0] - begin->trans[0]) * process;
	dst->trans[1]	= begin->trans[1] + (end->trans[0] - begin->trans[0]) * process;
	dst->rot		= begin->rot + (end->rot - begin->rot) * process;
	dst->scale[0]	= begin->scale[0] + (end->scale[0] - begin->scale[0]) * process;
	dst->scale[1]	= begin->scale[1] + (end->scale[1] - begin->scale[1]) * process;
}

void 
rg_joint_pose_identity(struct rg_joint_pose* pos) {
	pos->trans[0] = 0;
	pos->trans[1] = 0;
	pos->rot      = 0;
	pos->scale[0] = 1;
	pos->scale[1] = 1;
}

void 
rg_joint_pose_inv(const struct rg_joint_pose* src, struct rg_joint_pose* dst) {
	dst->trans[0] = - src->trans[0];
	dst->trans[1] = - src->trans[1];
	dst->rot      = - src->rot;
	dst->scale[0] = 1.0f / src->scale[0];
	dst->scale[1] = 1.0f / src->scale[1];
}

void
rg_local2world(const struct rg_joint_pose* src, const struct rg_joint_pose* local, struct rg_joint_pose* dst) {
	// scale
	dst->scale[0] = src->scale[0] * local->scale[0];
	dst->scale[1] = src->scale[1] * local->scale[1];
	// rot
	dst->rot      = src->rot + local->rot;
	// trans
	// dst.trans  = src.trans + sm::rotate_vector(local.trans * src.scale, src.rot);
	float tmp[2]  = { local->trans[0] * src->scale[0], local->trans[1] * src->scale[1] };
	_rotate_vector(tmp, src->rot, dst->trans);
	dst->trans[0] += src->trans[0];
	dst->trans[1] += src->trans[1];
}

void
rg_world2local(const struct rg_joint_pose* src, const struct rg_joint_pose* dst, struct rg_joint_pose* local) {
	// scale
	local->scale[0]	= dst->scale[0] / src->scale[0];
	local->scale[1]	= dst->scale[1] / src->scale[1];
	// rot
	local->rot		= dst->rot - src->rot;
	// trans
	// local.trans = sm::rotate_vector((dst.trans - src.trans), -src.rot) / src.scale;
	float tmp[2]	= { dst->trans[0] - src->trans[0], dst->trans[1] - src->trans[1] };
	_rotate_vector(tmp, -src->rot, local->trans);
	local->trans[0] /= src->scale[0];
	local->trans[1] /= src->scale[1];
}
