#include "rg_pose_mat.h"
#include "rg_pose_srt.h"

#include <string.h>
#include <math.h>

void 
rg_pose_mat_identity(struct rg_pose_mat* pose) {
	memset(pose->m, 0, sizeof(float) * 6);
	pose->m[0] = pose->m[3] = 1;
}

static inline void
_scale_mat(float* m, float sx, float sy) {
	if (sx != 1) {
		m[0] *= sx;
		m[2] *= sx;
		m[4] *= sx;
	}
	if (sy != 1) {
		m[1] *= sy;
		m[3] *= sy;
		m[5] *= sy;		
	}
}

static inline void
_rot_mat(float* m, float rot) {
	if (rot == 0) {
		return;
	}

	float cosd = cosf(rot);
	float sind = sinf(rot);

	float m0_cosd = m[0] * cosd;
	float m0_sind = m[0] * sind;
	float m1_cosd = m[1] * cosd;
	float m1_sind = m[1] * sind;
	float m2_cosd = m[2] * cosd;
	float m2_sind = m[2] * sind;
	float m3_cosd = m[3] * cosd;
	float m3_sind = m[3] * sind;
	float m4_cosd = m[4] * cosd;
	float m4_sind = m[4] * sind;
	float m5_cosd = m[5] * cosd;
	float m5_sind = m[5] * sind;

	m[0] = m0_cosd - m1_sind;
	m[1] = m0_sind + m1_cosd;
	m[2] = m2_cosd - m3_sind;
	m[3] = m2_sind + m3_cosd;
	m[4] = m4_cosd - m5_sind;
	m[5] = m4_sind + m5_cosd;
}

void
_build(float* m, float x, float y, float angle, float sx, float sy) {
	float c = cos(angle), s = sin(angle);

	m[0] = c * sx;
	m[1] = s * sx;
	m[2] = - s * sy;
	m[3] = c * sy;
	m[4] = x;
	m[5] = y;
}

void 
rg_pose_mat_build(struct rg_pose_mat* dst, const struct rg_pose_srt* src) {
	_build(dst->m, src->trans[0], src->trans[1], src->rot, src->scale[0], src->scale[1]);
}

static inline void
_mul(float* m, const float* m1, const float* m2) {
	m[0] = m1[0] * m2[0] + m1[1] * m2[2];
	m[1] = m1[0] * m2[1] + m1[1] * m2[3];
	m[2] = m1[2] * m2[0] + m1[3] * m2[2];
	m[3] = m1[2] * m2[1] + m1[3] * m2[3];
	m[4] = m1[4] * m2[0] + m1[5] * m2[2] + m2[4];
	m[5] = m1[4] * m2[1] + m1[5] * m2[3] + m2[5];
}

void 
rg_local2worldmat(const struct rg_pose_mat* src, const struct rg_pose_srt* local, struct rg_pose_mat* dst) {
	float m[6];
	_build(m, local->trans[0], local->trans[1], local->rot, local->scale[0], local->scale[1]);

	_mul(dst->m, m, src->m);
}