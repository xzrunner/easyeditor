#include "RenderCamera.h"

#include <sm_const.h>

#include <string.h>
#include <vector>
#include <float.h>

namespace s2
{

static const float HEIGHT_VAL = 1.414f;

RenderCamera::RenderCamera()
	: mode(CM_ORTHO)
	, base_y(0)
{}

RenderCamera RenderCamera::operator * (const RenderCamera& rc) const
{
	RenderCamera ret;
	if (rc.mode != CM_ORTHO) {
		ret.mode = rc.mode;
	} else {
		ret.mode = mode;
	}
	if (ret.mode == CM_PERSPECTIVE_AUTO_HEIGHT) {
		if (rc.base_y == FLT_MAX) {
			ret.base_y = rc.base_y;
			// todo pass spr
// 			std::vector<sm::vec2> bound;
// 			spr->GetBounding()->GetBoundPos(bound);
// 			for (int i = 0, n = bound.size(); i < n; ++i) {
// 				if (bound[i].y < ret.base_y) {
// 					ret.base_y = bound[i].y;
// 				}
// 			}
		} else {
			ret.base_y = rc.base_y;
		}
	}
	return ret;
}

void RenderCamera::CalculateZ(float cam_angle, sm::vec2 vertices[4], float z[4]) const
{
	if (mode == CM_ORTHO || mode == CM_PERSPECTIVE_NO_HEIGHT) {
		memset(z, 0, sizeof(float) * 4);
		return;
	}

	float zs = sin(cam_angle * SM_DEG_TO_RAD);

	float ymin = FLT_MAX, ymax = -FLT_MAX;
	for (int i = 0; i < 4; ++i) {
		float y = vertices[i].y;
		if (y < ymin) ymin = y;
		if (y > ymax) ymax = y;
	}

	//	float zoff = 0;
	if (base_y != FLT_MAX) {
		// 		assert(ymin >= base_y);
		// 		zoff = (ymin - base_y) * HEIGHT_VAL;

		ymin -= (ymin - base_y);
	}

	float height = (ymax - ymin) * HEIGHT_VAL;
	//	float height = ymax - ymin;
	for (int i = 0; i < 4; ++i) {
		float y = vertices[i].y;
		z[i] = -(y - ymin) / (ymax - ymin) * height * zs;
	}
}

}
