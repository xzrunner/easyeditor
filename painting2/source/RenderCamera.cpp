#include "painting2/RenderCamera.h"

#include <SM_Math.h>

#include <string.h>
#include <float.h>

namespace pt2
{

static const float HEIGHT_VAL = 1.414f;

RenderCamera::RenderCamera()
	: m_mode(CM_ORTHO)
	, m_base_y(0)
{
}

void RenderCamera::Reset()
{
	m_mode = CM_ORTHO;
	m_base_y = 0;
}

bool RenderCamera::operator == (const RenderCamera& cam) const
{
	return m_mode == cam.m_mode
		&& m_base_y == cam.m_base_y;
}

bool RenderCamera::operator != (const RenderCamera& cam) const
{
	return !(this->operator == (cam));
}

RenderCamera RenderCamera::operator * (const RenderCamera& rc) const
{
	RenderCamera ret;
	if (rc.m_mode != CM_ORTHO) {
		ret.m_mode = rc.m_mode;
	} else {
		ret.m_mode = m_mode;
	}
	if (ret.m_mode == CM_PERSPECTIVE_AUTO_HEIGHT) {
		if (rc.m_base_y == FLT_MAX) {
			ret.m_base_y = rc.m_base_y;
			// todo pass spr
// 			CU_VEC<sm::vec2> bound;
// 			spr->GetBounding().GetBoundPos(bound);
// 			for (int i = 0, n = bound.size(); i < n; ++i) {
// 				if (bound[i].y < ret.base_y) {
// 					ret.base_y = bound[i].y;
// 				}
// 			}
		} else {
			ret.m_base_y = rc.m_base_y;
		}
	}
	return ret;
}

void RenderCamera::CalculateZ(float cam_angle, const float vertices[8], float z[4]) const
{
	if (m_mode == CM_ORTHO || m_mode == CM_PERSPECTIVE_NO_HEIGHT) {
		memset(z, 0, sizeof(float) * 4);
		return;
	}

	float zs = sm::sin(cam_angle * SM_DEG_TO_RAD);

	float ymin = FLT_MAX, ymax = -FLT_MAX;
	for (int i = 0; i < 4; ++i) {
		float y = vertices[i * 2 + 1];
		if (y < ymin) ymin = y;
		if (y > ymax) ymax = y;
	}

	//	float zoff = 0;
	if (m_base_y != FLT_MAX) {
		// 		assert(ymin >= base_y);
		// 		zoff = (ymin - base_y) * HEIGHT_VAL;

		ymin -= (ymin - m_base_y);
	}

	float height = (ymax - ymin) * HEIGHT_VAL;
	//	float height = ymax - ymin;
	for (int i = 0; i < 4; ++i) {
		float y = vertices[i * 2 + 1];
		z[i] = -(y - ymin) / (ymax - ymin) * height * zs;
	}
}

}